#pragma once

#include "Core/Base.h"
#include "Core/String.h"

#include "Math/Vector.h"
#include "Math/Matrix.h"

#include <cstdint>
#include <unordered_map>
#include <optional>

namespace ERUNTIME_NAMESPACE::Renderer
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
    };
}