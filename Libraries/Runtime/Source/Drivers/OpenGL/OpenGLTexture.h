#pragma once

#include "Renderer/Texture.h"

#include <glad/glad.h>

#ifndef ERUNTIME_BUILD_DLL
#warning "You shouldn't include Driver headers from public API!"
#endif

// We don't include that in public API. We just can use `using namespace`
using namespace Renderer;

class OpenGLTexture : public Texture {
public:
    OpenGLTexture(const ResourceId& id, const TextureSpecification& spec);
    ~OpenGLTexture();

    const TextureSpecification& GetSpecification() const noexcept final { return m_specification; }

    Uint32 GetWidth() const noexcept final { return m_width; }
    Uint32 GetHeight() const noexcept final { return m_height; }
    
    void SetData(void* data, Uint32 size) final;

    void Bind(Uint32 slot) const final;

    Uint32 GetRendererId() const noexcept { return m_rendererId; }

    bool operator==(const Texture& other) const noexcept
    {
        return m_rendererId == dynamic_cast<const OpenGLTexture*>(&other)->m_rendererId;
    }

private:
    GLuint m_rendererId;
    TextureSpecification m_specification;
    Uint32 m_width;
    Uint32 m_height;

    GLenum m_dataFormat;
    GLenum m_internalFormat;
};