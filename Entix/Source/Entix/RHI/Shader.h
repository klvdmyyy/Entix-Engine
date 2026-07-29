#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Types.h"

#include "Entix/Core/Result.h"

#include "Entix/Resources/Resource.h"

#include <vector>
#include <bitset>
#include <filesystem>
#include <functional>

namespace Entix::RHI
{
    enum class ShaderStage
    {
        Vertex   = 1 << 0,
        Fragment = 1 << 1,
    };

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

        [[nodiscard]]
        virtual const char* GetStageEntryPoint(ShaderStage stage) const noexcept = 0;
    };

    class Shader : public Resource
    {
    public:
        virtual ~Shader() = default;

        virtual Result<void> EachStage(std::function<Result<void>(ShaderStage stage)> callback) const noexcept
        {
            for(ShaderStage stage : {
                ShaderStage::Vertex,
                ShaderStage::Fragment
            })
            {
                if(HasStage(stage))
                {
                    EX_TRY(callback(stage));
                }
            }

            return {};
        }
        virtual bool HasStage(ShaderStage stage) const noexcept = 0;
    };
}