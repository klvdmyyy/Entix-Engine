#include "Drivers/OpenGL/OpenGLTexture.h"

#include "Core/Assert.h"

#include <tracy/Tracy.hpp>

namespace Utils {
    static GLenum EntixImageFormatToGLDataFormat(ImageFormat format)
    {
        switch(format)
        {
            case ImageFormat::RGB8: return GL_RGB;
            case ImageFormat::RGBA8: return GL_RGBA;
        }

        EX_ASSERT(false, "Unsupported image format!");
        return 0;
    }

    static GLenum EntixImageFormatToGLInternalFormat(ImageFormat format)
    {
        switch(format)
        {
            case ImageFormat::RGB8: return GL_RGB8;
            case ImageFormat::RGBA8: return GL_RGBA8;
        }

        EX_ASSERT(false, "Unsupported image format!");
        return 0;
    }
}

OpenGLTexture::OpenGLTexture(const ResourceId& id, const TextureSpecification& spec)
    : Texture(id), m_specification(spec), m_width(m_specification.width), m_height(m_specification.height)
{
    ZoneScoped;

    m_dataFormat = Utils::EntixImageFormatToGLDataFormat(m_specification.format);
    m_internalFormat = Utils::EntixImageFormatToGLInternalFormat(m_specification.format);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
    glTextureStorage2D(m_rendererId, 1, m_internalFormat, m_width, m_height);

    glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture::~OpenGLTexture()
{
    glDeleteTextures(1, &m_rendererId);
}

void OpenGLTexture::SetData(void* data, Uint32 size)
{
    ZoneScoped;

    SetMemorySize(Bytes(size));

    Uint32 bpp = m_dataFormat == GL_RGBA ? 4 : 3;
    EX_ASSERT(size == m_width * m_height * bpp, "Data must be entire texture!");
    glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture::Bind(Uint32 slot) const
{
    ZoneScoped;

    glBindTextureUnit(slot, m_rendererId);
}