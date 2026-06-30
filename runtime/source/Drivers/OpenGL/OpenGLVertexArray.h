// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"

#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

#include <glad/glad.h>

// We don't include that in public API. We just can use `using namespace`
using namespace ERUNTIME_NAMESPACE::Renderer;

namespace ERUNTIME_NAMESPACE
{
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
}