#include "Drivers/OpenGL/OpenGLShader.h"

#include "Core/Assert.h"

#include "Core/IO/Decorators.h"
#include "Core/IO/FileReader.h"

#include "Math/TypePtr.h"

#include <tracy/Tracy.hpp>

#include <vector>

static GLenum ShaderStageToGLenum(ShaderStage stage) {
    switch (stage) {
    case ShaderStage::Vertex:
        return GL_VERTEX_SHADER;
    case ShaderStage::Fragment:
        return GL_FRAGMENT_SHADER;
    }

    EX_ASSERT(false, "Unknown shader stage!");
}

static GLuint CompileOpenGLShader(const std::string &src,
                                  GLenum shader_type) {
    ZoneScoped;
    GLuint shader = glCreateShader(shader_type);

    const GLchar *source = static_cast<const GLchar *>(src.c_str());
    glShaderSource(shader, 1, &source, 0);

    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLenght = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLenght);

        std::vector<GLchar> infoLog(maxLenght);
        glGetShaderInfoLog(shader, maxLenght, &maxLenght, &infoLog[0]);

        glDeleteShader(shader);

        EX_ASSERT(false, "failed to compile shader. error: {}", infoLog.data());

        /* TODO: Error log + Assertion!!!!!! Failed to compile shader. Info message
         * in `info_log` variable */
    }

    return shader;
}

OpenGLShader::OpenGLShader(const ResourceId& id)
    : Shader(id)
{
}

bool OpenGLShader::CompileFromStream(IO::Reader& shaderReader)
{
    ZoneScoped;
    
    if(m_isCompiled) {
        EX_LOG(Info, LogCategory::Driver, "OpenGL program is already linked. Force reloading shaders!");

        m_isCompiled = false;
        glDeleteProgram(m_program);
    }

    auto source = IO::TextReader::CreateNonOwned(shaderReader).ReadAll();

    auto shaderSourceMap = GLSL_Preprocess(source);

    auto &vertexSource = shaderSourceMap[ShaderStage::Vertex];
    auto &fragmentSource = shaderSourceMap[ShaderStage::Fragment];

    auto vertShader = CompileOpenGLShader(vertexSource, GL_VERTEX_SHADER);
    auto fragShader =
        CompileOpenGLShader(fragmentSource, GL_FRAGMENT_SHADER);

    m_program = glCreateProgram();

    glAttachShader(m_program, vertShader);
    glAttachShader(m_program, fragShader);
    glLinkProgram(m_program);

    GLint isLinked = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
        {
            GLint maxLenght = 0;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLenght);

            std::vector<GLchar> infoLog(maxLenght);
            glGetProgramInfoLog(m_program, maxLenght, &maxLenght, &infoLog[0]);

            glDeleteProgram(m_program);
            glDeleteShader(vertShader);
            glDeleteShader(fragShader);

            EX_ASSERT(false, "failed to link shader program. error: {}",
                            infoLog.data());

            return false;

            /* TODO: Error log + Assertion!!!!!! Failed to link program. Info message in `info_log` variable */
        }

    glDetachShader(m_program, vertShader);
    glDetachShader(m_program, fragShader);

    return true;
}

OpenGLShader::~OpenGLShader()
{ glDeleteProgram(m_program);
}

void OpenGLShader::Bind() const
{ glUseProgram(m_program);
}

void OpenGLShader::Unbind() const
{
    glUseProgram(0);
}

void OpenGLShader::SetInt(const String& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void OpenGLShader::SetIntArray(const String& name, int* values, Uint32 count)
{
    glUniform1iv(GetUniformLocation(name), count, values);
}

void OpenGLShader::SetFloat(const String& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void OpenGLShader::SetFloat2(const String& name, Float2 value)
{
    glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void OpenGLShader::SetFloat3(const String& name, Float3 value)
{
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void OpenGLShader::SetFloat4(const String& name, Float4 value)
{
    glUniform4f(GetUniformLocation(name),
                value.x, value.y,
                value.z, value.w);
}

void OpenGLShader::SetFloat4x4(const String& name, Float4x4 value)
{
    glUniformMatrix4fv(GetUniformLocation(name),
                       1, GL_FALSE, Math::ValuePtr(value));
}

GLint OpenGLShader::GetUniformLocation(const String& name)
{
    return glGetUniformLocation(m_program, name.c_str());
}

const String& OpenGLShader::GetName() const
{
    return k_name;
}