#pragma once

#include "Core/Assert.h"
#include "Core/Base.h"
#include "Core/String.h"
#include "Core/Types.h"

#include <vector>

namespace Renderer
{
    enum class ShaderDataType {
        None = 0,
        Bool,
        Int,
        Int2,
        Int3,
        Int4,
        Float,
        Float2,
        Float3,
        Float4,
        Float3x3,
        Float4x4
    };

    [[nodiscard]]
    static Uint32 ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type) {
        case ShaderDataType::None:
            return 0;
        case ShaderDataType::Bool:
            return 1;
        case ShaderDataType::Int:
            return 4;
        case ShaderDataType::Int2:
            return 4 * 2;
        case ShaderDataType::Int3:
            return 4 * 3;
        case ShaderDataType::Int4:
            return 4 * 4;
        case ShaderDataType::Float:
            return 4;
        case ShaderDataType::Float2:
            return 4 * 2;
        case ShaderDataType::Float3:
            return 4 * 3;
        case ShaderDataType::Float4:
            return 4 * 4;
        case ShaderDataType::Float3x3:
            return 4 * 3 * 3;
        case ShaderDataType::Float4x4:
            return 4 * 4 * 4;
        }

        EX_ASSERT(false, "Unknown ShaderDataType: {}", static_cast<int>(type));
        return 0;
    }

    struct ERUNTIME_API BufferElement
    {
        String name;
        ShaderDataType type;
        Uint32 size;
        size_t offset;
        bool normalized;

        BufferElement(const String& name, ShaderDataType type, bool normalized = false)
            : name(name), type(type), size(ShaderDataTypeSize(type)), offset(0),
              normalized(normalized)
        {
        }

        [[nodiscard]]
        Uint32 GetComponentCount() const
        {
            switch (type) {
            case ShaderDataType::None:
                return 0;
            case ShaderDataType::Bool:
                return 1;
            case ShaderDataType::Int:
                return 1;
            case ShaderDataType::Int2:
                return 2;
            case ShaderDataType::Int3:
                return 3;
            case ShaderDataType::Int4:
                return 4;
            case ShaderDataType::Float:
                return 1;
            case ShaderDataType::Float2:
                return 2;
            case ShaderDataType::Float3:
                return 3;
            case ShaderDataType::Float4:
                return 4;
            case ShaderDataType::Float3x3:
                return 3 * 3;
            case ShaderDataType::Float4x4:
                return 4 * 4;
            }

            EX_ASSERT(false, "Unknown ShaderDataType: {}", static_cast<int>(type));
            return 0;
        }
    };

    class ERUNTIME_API BufferLayout
    {
    public:
        BufferLayout() { }
        BufferLayout(const std::initializer_list<BufferElement>& elements)
            : m_elements(elements)
        {
            CalculateOffsetAndStride();
        }

        [[nodiscard]]
        uint32_t GetStride() const noexcept
        {
            return m_stride;
        }

        const std::vector<BufferElement> &GetElements() const {
          return m_elements;
        }

        std::vector<BufferElement>::iterator begin() {
          return m_elements.begin();
        }
        std::vector<BufferElement>::iterator end() { return m_elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const {
          return m_elements.begin();
        }
        std::vector<BufferElement>::const_iterator end() const {
          return m_elements.end();
        }

    private:
        void CalculateOffsetAndStride()
        {
            Uint32 offset = 0;
            m_stride = 0;
            for(auto& element : m_elements)
            {
                element.offset = offset;
                offset += element.size;
                m_stride += element.size;
            }
        }

        std::vector<BufferElement> m_elements;
        Uint32 m_stride = 0;
    };

    class ERUNTIME_API VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(const void* data, Uint32 size) = 0;

        [[nodiscard]]
        virtual Uint32 GetCount() const noexcept = 0;

        [[nodiscard]]
        virtual const BufferLayout &GetLayout() const = 0;

        virtual void SetLayout(const BufferLayout& layout) = 0;
    };

    class ERUNTIME_API IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        [[nodiscard]]
        virtual Uint32 GetCount() const = 0;
    };
}