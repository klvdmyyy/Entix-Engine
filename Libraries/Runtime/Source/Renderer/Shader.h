#pragma once

#include "Core/Base.h"
#include "Core/String.h"
#include "Core/Types.h"

#include "Math/Vector.h"
#include "Math/Matrix.h"

#include <cstdint>
#include <unordered_map>
#include <optional>

namespace Renderer
{
    enum class ShaderStage
    {
        Vertex,   //< Vertex Shader Stage
        Fragment, //< Fragment(Pixel) Shader Stage
    };

    ERUNTIME_API std::optional<ShaderStage> ShaderStageFromString(const String& s);

    /**
     * GLSL Shader preprocessing
     * =========================
     *
     * Split one shader file to different shader code by specific stages. (vertex, fragment, ...)
     *
     * Specify shader stages by following derictives:
     *
     * #type vertex
     * 
     * #type fragment
     */
    ERUNTIME_API std::unordered_map<ShaderStage, String> GLSL_Preprocess(const String& source);

    class ERUNTIME_API Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetInt(const String& name, int value) = 0;
        virtual void SetIntArray(const String& name, int* values, Uint32 count) = 0;
        virtual void SetFloat(const String& name, float value) = 0;
        virtual void SetFloat2(const String& name, Float2 value) = 0;
        virtual void SetFloat3(const String& name, Float3 value) = 0;
        virtual void SetFloat4(const String& name, Float4 value) = 0;
        virtual void SetFloat4x4(const String& name, Float4x4 value) = 0;

        [[nodiscard]]
        virtual const String& GetName() const = 0;
    };
}