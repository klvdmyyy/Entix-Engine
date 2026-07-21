#pragma once

#include "Renderer/Framebuffer.h"

#include <optional>

#include <glad/glad.h>

#ifndef ERUNTIME_BUILD_DLL
#warning "You shouldn't include Driver headers from public API!"
#endif

// We don't include that in public API. We just can use `using namespace`
using namespace Renderer;

class ERUNTIME_API OpenGLFramebuffer : public Framebuffer {
public:
    OpenGLFramebuffer(const FramebufferSpecification& spec);
    ~OpenGLFramebuffer();

    void Invalidate();

    void Bind() final;
    void Unbind() final;

    void Resize(Uint32 width, Uint32 height) final;
    int ReadPixel(Uint32 attachmentIndex, Int32 x, Int32 y) final;

    void ClearAttachment(Uint32 attachmentIndex, Int32 value) final;

    Uint32 GetRendererId() const noexcept final { return m_rendererId; }
    Uint32 GetColorAttachmentRendererId(Uint32 index = 0) const final {
        EX_DEBUG_ASSERT_EXPR(index < m_colorAttachments.size());
        return m_colorAttachments.at(index);
    };

    const FramebufferSpecification& GetSpecification() const noexcept final { return m_specification; }

private:
    GLuint m_rendererId = 0;

    FramebufferSpecification m_specification;

    std::vector<FramebufferTextureSpecification> m_colorAttachmentSpecifications;
    std::optional<FramebufferTextureSpecification> m_depthAttachmentSpecification = std::nullopt;

    std::vector<GLuint> m_colorAttachments;
    GLuint m_depthAttachment = 0;
};