#include "Drivers/OpenGL/OpenGLVertexArray.h"

#include "Drivers/OpenGL/OpenGLContext.h"

static GLenum ShaderDataTypeToGLenum(ShaderDataType type)
{
    switch (type) {
    case ShaderDataType::Bool:
        return GL_BOOL;
    case ShaderDataType::Int:
        return GL_INT;
    case ShaderDataType::Int2:
        return GL_INT;
    case ShaderDataType::Int3:
        return GL_INT;
    case ShaderDataType::Int4:
        return GL_INT;
    case ShaderDataType::Float:
        return GL_FLOAT;
    case ShaderDataType::Float2:
        return GL_FLOAT;
    case ShaderDataType::Float3:
        return GL_FLOAT;
    case ShaderDataType::Float4:
        return GL_FLOAT;
    case ShaderDataType::Float3x3:
        return GL_FLOAT;
    case ShaderDataType::Float4x4:
        return GL_FLOAT;
    default:
        break;
    }

    EX_ASSERT(false, "Unknown ShaderDataType: {}", static_cast<int>(type));
    return 0;
}

OpenGLVertexArray::OpenGLVertexArray(const ResourceId& id) : VertexArray(id)
{
    glCreateVertexArrays(1, &m_vao);
}

OpenGLVertexArray::~OpenGLVertexArray()
{ glDeleteVertexArrays(1, &m_vao);
}

void OpenGLVertexArray::Bind() const
{ glBindVertexArray(m_vao);
}

void OpenGLVertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{
    EX_ASSERT(vertexBuffer->GetLayout().GetElements().size(),
              "vertex buffer has no layout!");

    this->Bind();

    vertexBuffer->Bind();

    uint32_t index = 0;
    const auto &layout = vertexBuffer->GetLayout();

    for(const auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, element.GetComponentCount(),
                                  ShaderDataTypeToGLenum(element.type),
                                  element.normalized ? GL_TRUE : GL_FALSE,
                                  layout.GetStride(),
                                  reinterpret_cast<const void *>(element.offset));
            index++;
        }

    m_vertexBuffers.push_back(vertexBuffer);

    vertexBuffer->Unbind();

    this->Unbind();
}

void
OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
{
    this->Bind();

    indexBuffer->Bind();

    m_indexBuffer = indexBuffer;

    this->Unbind();

    indexBuffer->Unbind();
}