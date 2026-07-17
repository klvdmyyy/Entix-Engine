#include "Renderer/Shader.h"

#include "Core/Assert.h"

#include <tracy/Tracy.hpp>

#include <cstring>

namespace Renderer
{
    std::optional<ShaderStage> ShaderStageFromString(const String& s)
    {
        const auto& stage = s;
        if(s == "vertex") return ShaderStage::Vertex;
        else if(s == "fragment") return ShaderStage::Fragment;
        else return std::nullopt;
    }

    std::unordered_map<ShaderStage, String> GLSL_Preprocess(const String& source)
    {
        ZoneScoped;
        
        std::unordered_map<ShaderStage, String> shader_sources;

        const char* type_token = "#type";

        size_t type_token_length = strlen(type_token);
        size_t pos = source.find(type_token, 0);

        while(pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);

            EX_ASSERT(eol != std::string::npos, "syntax error");

            size_t begin = pos + type_token_length + 1;
            String type = source.substr(begin, eol - begin);

            EX_ASSERT(ShaderStageFromString(type),
                     "invalid shader stage specified!");

            size_t next_line_pos = source.find_first_not_of("\r\n", eol);

            EX_ASSERT(next_line_pos != std::string::npos, "syntax error");

            pos = source.find(type_token, next_line_pos);

            shader_sources[ShaderStageFromString(type).value()] =
                (pos == std::string::npos)
                    ? source.substr(next_line_pos)
                    : source.substr(next_line_pos, pos - next_line_pos);
        }

        return shader_sources;
    }
}