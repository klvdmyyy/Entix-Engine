#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Types.h"

#include "Entix/Core/Result.h"

#include <vector>
#include <bitset>
#include <filesystem>

namespace Entix::RHI
{
    enum class ShaderStage
    {
        Vertex   = 1 << 0,
        Fragment = 1 << 1,
    };

    Uint8 operator&(Uint8 num, ShaderStage stage)
    {
        return num & static_cast<Uint8>(stage);
    }

    Uint8 operator|(Uint8 num, ShaderStage stage)
    {
        return num | static_cast<Uint8>(stage);
    }

    Uint8 operator|=(Uint8 num, ShaderStage stage)
    {
        return num | stage;
    }

    Uint8 operator|(ShaderStage first, ShaderStage second)
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

    class Shader
    {
    public:
        virtual ~Shader() = default;
    };
}