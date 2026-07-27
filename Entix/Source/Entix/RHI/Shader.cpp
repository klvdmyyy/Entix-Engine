#include "Entix/RHI/Shader.h"

#include "Entix/Core/Debug/Logger.h"

#include "Entix/Core/IO/Stream.h"
#include "Entix/Core/IO/FileStream.h"
#include "Entix/Core/IO/TextStream.h"

#include "Entix/Core/Globals.h"

#include <slang/slang.h>
#include <slang/slang-com-ptr.h>
#include <slang/slang-com-helper.h>

#define EX_SLANG_TRY(EXPR) \
    if(!SLANG_SUCCEEDED(EXPR)) \
        return Error(String("Following Slang expression fail: " + String(#EXPR)))

namespace Entix::RHI
{
    class SlangShaderCompiler : public ShaderCompiler
    {
    public:
        SlangShaderCompiler()
        {
            Initialize().Unwrap();
        }

        ~SlangShaderCompiler()
        {
            Shutdown().Unwrap();
        }

        Result<ShaderCompilationData> Compile(const std::filesystem::path& filepath) noexcept final
        {
            ShaderCompilationData res;

            auto file = CreateScope<IO::FileStream>(filepath, IO::StreamMode::Read);
            auto input = CreateScope<IO::TextStream>(std::move(file));

            EX_LET_TRY(source, input->ReadAll());

            

            return res;
        }

        [[nodiscard]]
        constexpr const char* GetStageEntryPoint(ShaderStage stage) const noexcept
        {
            switch(stage)
            {
                case ShaderStage::Vertex: return "VertMain";
                case ShaderStage::Fragment: return "FragMain";
            }

            Panic("Unknown shader stage. This error shouldn't be caused, it's Engine issue!");
        }

    private:
        Result<void> Initialize()
        {
            slang::createGlobalSession(m_globalSession.writeRef());

            slang::TargetDesc spirvTargetDesc;
            spirvTargetDesc.format = SLANG_SPIRV;
            spirvTargetDesc.profile = m_globalSession->findProfile("spirv_1_5");

            slang::SessionDesc sessionDesc;
            sessionDesc.targets = &spirvTargetDesc;
            sessionDesc.targetCount = 1;

            EX_SLANG_TRY(m_globalSession->createSession(sessionDesc, m_session.writeRef()));

            return {};
        }

        Result<void> Shutdown()
        {
            return {};
        }

        Slang::ComPtr<slang::IGlobalSession> m_globalSession;
        Slang::ComPtr<slang::ISession> m_session;
    };

    ShaderCompiler* ShaderCompiler::Instance()
    {
        static SlangShaderCompiler s_instance;
        return &s_instance;
    }

    Result<std::vector<std::byte>> CompileShader(IO::Stream& input)
    {
        Slang::ComPtr<slang::IGlobalSession> globalSession;
        slang::createGlobalSession(globalSession.writeRef());

        slang::TargetDesc targetDesc{};
        targetDesc.format = SLANG_SPIRV;
        targetDesc.profile = globalSession->findProfile("spirv_1_5");

        slang::SessionDesc sessionDesc{};
        sessionDesc.targets = &targetDesc;
        sessionDesc.targetCount = 1;

        Slang::ComPtr<slang::ISession> session;
        globalSession->createSession(sessionDesc, session.writeRef());

        EX_LET_TRY(shaderCode, IO::TextStream::CreateNonOwned(input).ReadAll());

        Slang::ComPtr<slang::IBlob> diagnosticBlob;
        slang::IModule* mod = session->loadModuleFromSourceString(
            "SimpleShader",
            "SimpleShader.slang",
            shaderCode.c_str()
            // diagnosticBlob.writeRef()
        );

        slang::IEntryPoint* vertEntry;
        mod->findEntryPointByName("vertMain", &vertEntry);
        
        slang::IEntryPoint* fragEntry;
        mod->findEntryPointByName("fragMain", &fragEntry);

        Slang::ComPtr<slang::IComponentType> linkedProgram;
        slang::IComponentType* components[] = {mod, vertEntry, fragEntry};
        session->createCompositeComponentType(components, sizeof(components), linkedProgram.writeRef());

        Slang::ComPtr<slang::IBlob> codeBlob;
        linkedProgram->getEntryPointCode(0, 0, codeBlob.writeRef(), diagnosticBlob.writeRef());

        return std::vector<std::byte>(
            reinterpret_cast<const std::byte*>(codeBlob->getBufferPointer()),
            reinterpret_cast<const std::byte*>(codeBlob->getBufferPointer()) + codeBlob->getBufferSize()
        );
    }
}