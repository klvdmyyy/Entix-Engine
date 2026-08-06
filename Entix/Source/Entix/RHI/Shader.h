#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Types.h"
#include "Entix/Core/String.h"

#include "Entix/Core/Result.h"

#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"

#include "Entix/Resources/Resource.h"

#include <vector>
#include <filesystem>
#include <functional>

namespace Entix
{
    namespace RHI
    {
        enum class ShaderStage
        {
            Vertex   = EX_BIT(0),
            Fragment = EX_BIT(1),
        };
    }
    
    template<>
    ENTIX_API String ToString<RHI::ShaderStage>(const RHI::ShaderStage& stage);
}

namespace Entix::RHI
{
    inline Uint8 operator&(Uint8 num, ShaderStage stage)
    {
        return num & static_cast<Uint8>(stage);
    }

    inline Uint8 operator|(Uint8 num, ShaderStage stage)
    {
        return num | static_cast<Uint8>(stage);
    }

    inline Uint8 operator|=(Uint8 num, ShaderStage stage)
    {
        return num | stage;
    }

    inline Uint8 operator|(ShaderStage first, ShaderStage second)
    {
        return static_cast<Uint8>(first) | static_cast<Uint8>(second);
    }

    struct ShaderCompilationData
    {
        Uint8 stages = 0;
        std::vector<std::byte> code;
    };

    class ShaderCompiler
    {
    public:
        /**
         * @brief It's only one right way to get ShaderCompiler.
         */
        [[nodiscard]]
        ENTIX_API static ShaderCompiler* Instance();

        virtual ~ShaderCompiler() = default;

        [[nodiscard]]
        virtual Result<ShaderCompilationData> Compile(const std::filesystem::path& filepath) noexcept = 0;

        virtual Result<void> RecreateSession() = 0;

        [[nodiscard]]
        virtual const char* GetStageEntryPoint(ShaderStage stage) const noexcept = 0;
    };

    class Shader : public Resource
    {
    public:
        Shader(const ResourceId& resourceId) : Resource(resourceId) {}
        virtual ~Shader() = default;

        void EachStage(std::function<void(ShaderStage)> callback)
        {
            for(auto stage : {
                ShaderStage::Vertex,
                ShaderStage::Fragment
            })
            {
                if(HasStage(stage))
                {
                    callback(stage);
                }
                else
                {
                    EX_LOG(LogRHI, Debug, "No {} stage in {}", ToString(stage), GetResourceId().GetFilenameString());
                }
            }
        }
        virtual bool HasStage(ShaderStage stage) const noexcept = 0;
    };
}

namespace Entix
{
    
}