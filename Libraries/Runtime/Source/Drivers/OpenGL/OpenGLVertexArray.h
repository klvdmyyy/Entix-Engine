// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"

#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

#include <glad/glad.h>

#ifndef ERUNTIME_BUILD_DLL
#warning "You shouldn't include Driver headers from public API!"
#endif

// We don't include that in public API. We just can use `using namespace`
using namespace Renderer;

class ERUNTIME_API OpenGLVertexArray : public VertexArray
{
 public:
    OpenGLVertexArray();
    ~OpenGLVertexArray();

    void Bind() const final;
    void Unbind() const final;

    void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) final;
    void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) final;

    [[nodiscard]]
    const std::vector<Ref<VertexBuffer>> &GetVertexBuffers() const final {
        return m_vertexBuffers;
    }
    const Ref<IndexBuffer> &GetIndexBuffer() const final {
        return m_indexBuffer;
    }

 private:
    GLuint m_vao;

    std::vector<Ref<VertexBuffer>> m_vertexBuffers;
    Ref<IndexBuffer> m_indexBuffer;
};