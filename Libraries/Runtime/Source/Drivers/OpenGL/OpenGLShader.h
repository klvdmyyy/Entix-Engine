// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/String.h"

#include "Renderer/Shader.h"

#include <glad/glad.h>

#ifndef ERUNTIME_BUILD_DLL
#warning "You shouldn't include Driver headers from public API!"
#endif

// We don't include that in public API. We just can use `using namespace`
using namespace Renderer;

class ERUNTIME_API OpenGLShader : public Shader
{
 public:
    OpenGLShader(IO::Reader& shaderReader);
    ~OpenGLShader();

    void Bind() const final;
    void Unbind() const final;

    void SetInt(const String& name, int value) final;
    void SetIntArray(const String& name, int* values, Uint32 count) final;
    void SetFloat(const String& name, float value) final;
    void SetFloat2(const String& name, Float2 value) final;
    void SetFloat3(const String& name, Float3 value) final;
    void SetFloat4(const String& name, Float4 value) final;
    void SetFloat4x4(const String& name, Float4x4 value) final;

    [[nodiscard]]
    const String& GetName() const final;

    [[nodiscard]]
    GLint GetUniformLocation(const String& name);

 private:
    GLuint m_program;
    const String k_name = String();
};