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

#define EX_SLANG_CHECK_DIAGNOSTIC(VAR) \
    do { if(VAR) \
    { \
        String msg( \
            reinterpret_cast<const char*>(VAR->getBufferPointer()), \
            reinterpret_cast<const char*>(VAR->getBufferPointer()) + VAR->getBufferSize() \
        ); \
        return Error(msg); \
    } } while(0)

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

            EX_LET_TRY(shaderCode, input->ReadAll());

            Slang::ComPtr<slang::IBlob> diagnosticBlob;

            slang::IModule* mod = m_session->loadModuleFromSourceString(
                filepath.filename().replace_extension().string().c_str(),
                filepath.filename().string().c_str(),
                shaderCode.c_str(),
                diagnosticBlob.writeRef()
            );

            EX_SLANG_CHECK_DIAGNOSTIC(diagnosticBlob);

            slang::IEntryPoint* vertEntry;
            EX_SLANG_TRY(mod->findEntryPointByName(GetStageEntryPoint(ShaderStage::Vertex), &vertEntry));
            
            slang::IEntryPoint* fragEntry;
            EX_SLANG_TRY(mod->findEntryPointByName(GetStageEntryPoint(ShaderStage::Fragment), &fragEntry));

            Slang::ComPtr<slang::IComponentType> linkedProgram;
            slang::IComponentType* components[] = {mod, vertEntry, fragEntry};
            EX_SLANG_TRY(m_session->createCompositeComponentType(components, sizeof(components) / sizeof(slang::IComponentType*), linkedProgram.writeRef()));

            Slang::ComPtr<slang::IBlob> codeBlob;
            EX_SLANG_TRY(linkedProgram->getEntryPointCode(0, 0, codeBlob.writeRef(), diagnosticBlob.writeRef()));

            EX_SLANG_CHECK_DIAGNOSTIC(diagnosticBlob);

            res.code = std::vector<std::byte>(
                reinterpret_cast<const std::byte*>(codeBlob->getBufferPointer()),
                reinterpret_cast<const std::byte*>(codeBlob->getBufferPointer()) + codeBlob->getBufferSize()
            );

            slang::ProgramLayout* programLayout = linkedProgram->getLayout(0);

            Usize entryPointCount = programLayout->getEntryPointCount();

            for(Usize i = 0; i < entryPointCount; i++)
            {
                slang::EntryPointLayout* entryPointLayout = programLayout->getEntryPointByIndex(i);
                if(entryPointLayout)
                {
                    SlangStage stage = entryPointLayout->getStage();

                    switch(stage)
                    {
                        case SLANG_STAGE_VERTEX:
                            res.stages |= ShaderStage::Vertex;
                            EX_LOG(
                                ShaderCompilation, Trace,
                                "Vertex stage entry point found in '{}'",
                                filepath.filename().string()
                            );
                            break;
                        
                        case SLANG_STAGE_FRAGMENT:
                            res.stages |= ShaderStage::Fragment;
                            EX_LOG(
                                ShaderCompilation, Trace,
                                "Fragment stage entry point found in '{}'",
                                filepath.filename().string()
                            );
                            break;
                        
                        default:
                            EX_LOG(
                                ShaderCompilation, Warning,
                                "Unresolved stage entry point in '{}': {}",
                                filepath.filename().string(), static_cast<Uint16>(stage)
                            );
                            break;
                    }
                }
            }

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
}