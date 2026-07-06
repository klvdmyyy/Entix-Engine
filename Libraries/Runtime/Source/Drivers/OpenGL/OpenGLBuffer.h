// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"

#include "Renderer/Buffer.h"

#include <glad/glad.h>

#ifndef ERUNTIME_BUILD_DLL
#warning "You shouldn't include Driver headers from public API!"
#endif

// We don't include that in public API. We just can use `using namespace`
using namespace Renderer;

class ERUNTIME_API OpenGLVertexBuffer : public VertexBuffer
{
 public:
    OpenGLVertexBuffer(float* data, uint32_t size);
    ~OpenGLVertexBuffer();

    void Bind() const final;
    void Unbind() const final;

    const BufferLayout &GetLayout() const noexcept final {
        return m_layout;
    }
    void SetLayout(const BufferLayout &layout) final { m_layout = layout; }

 private:
    GLuint m_vbo;
    BufferLayout m_layout;
};

class ERUNTIME_API OpenGLIndexBuffer : public IndexBuffer
{
 public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
    ~OpenGLIndexBuffer();

    void Bind() const final;
    void Unbind() const final;

    uint32_t GetCount() const final { return m_count; }
    
 private:
    GLuint m_ebo;
    uint32_t m_count;
};