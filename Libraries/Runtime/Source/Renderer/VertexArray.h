#pragma once

#include "Core/Base.h"
#include "Core/Memory.h"

#include "Renderer/Buffer.h"

#include "Resources/Resource.h"

#include <vector>

namespace Renderer
{
    class ERUNTIME_API VertexArray : public Resource
    {
    public:
        VertexArray(const ResourceId& id) : Resource(id) {}
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) = 0;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) = 0;

        [[nodiscard]]
        virtual Uint32 GetVertexCount() const noexcept = 0;

        [[nodiscard]]
        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;

        [[nodiscard]]
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;
    };
}