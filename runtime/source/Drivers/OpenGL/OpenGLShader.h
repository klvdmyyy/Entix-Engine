// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/String.h"

#include "Renderer/Shader.h"

#include <glad/glad.h>

// We don't include that in public API. We just can use `using namespace`
using namespace ERUNTIME_NAMESPACE::Renderer;

namespace ERUNTIME_NAMESPACE
{
    class ERUNTIME_API OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const String& filepath);
        ~OpenGLShader();

        void Bind() const final;
        void Unbind() const final;

    private:
        GLuint m_program;
    };
}