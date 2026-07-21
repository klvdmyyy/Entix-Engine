#include "Drivers/OpenGL/OpenGLFramebuffer.h"

#include "Core/Assert.h"

static constexpr auto MAX_FRAMEBUFFER_SIZE = 8192U;

namespace Utils {
    static GLenum TextureTarget(bool multisampled)
    {
        return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static void CreateTextures(bool multisampled, Uint32* outId, Uint32 count)
    {
        glCreateTextures(TextureTarget(multisampled), count, outId);
    }

    static void BindTexture(bool multisampled, Uint32 id)
    {
        glBindTexture(TextureTarget(multisampled), id);
    }

    static void AttachColorTexture(Uint32 id, Int32 samples, GLenum internalFormat, GLenum format, Uint32 width, Uint32 height, Int32 index)
    {
        bool multisampled = samples > 1;
        if(multisampled)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
    }

    static void AttachDepthTexture(Uint32 id, Int32 samples, GLenum format, GLenum attachmentType, Uint32 width, Uint32 height)
    {
        bool multisampled = samples > 1;
        if (multisampled)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
        }
        else
        {
            glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
    }

    static bool IsDepthFormat(FramebufferTextureFormat format)
    {
        switch(format)
        {
            case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
        }

        return false;
    }

    static GLenum EntixFBTextureFormatToGL(FramebufferTextureFormat format)
    {
        switch(format)
        {
            case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
            case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
        }

        EX_ASSERT(false, "Unsupported framebuffer texture format provided.");
        return 0;
    }
}

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
    : m_specification(spec)
{
    for(auto spec : m_specification.attachmentSpec.attachments) {
        if(!Utils::IsDepthFormat(spec.textureFormat)) {
            m_colorAttachmentSpecifications.emplace_back(spec);
        } else {
            m_depthAttachmentSpecification = spec;
        }
    }

    Invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
    glDeleteFramebuffers(1, &m_rendererId);
    glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
    glDeleteTextures(1, &m_depthAttachment);
}

void OpenGLFramebuffer::Invalidate()
{
    if(m_rendererId)
    {
        glDeleteFramebuffers(1, &m_rendererId);
        glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
        glDeleteTextures(1, &m_depthAttachment);

        m_colorAttachments.clear();
        m_depthAttachment = 0;
    }

    glCreateFramebuffers(1, &m_rendererId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);

    bool multisample = m_specification.samples > 1;

    // Attachments
    if(m_colorAttachments.size())
    {
        m_colorAttachments.resize(m_colorAttachmentSpecifications.size());
        Utils::CreateTextures(multisample, m_colorAttachments.data(), m_colorAttachments.size());

        for(size_t i = 0; i < m_colorAttachments.size(); i++)
        {
            Utils::BindTexture(multisample, m_colorAttachments[i]);
            switch(m_colorAttachmentSpecifications[i].textureFormat)
            {
                case FramebufferTextureFormat::RGBA8:
                    Utils::AttachColorTexture(m_colorAttachments[i], m_specification.samples, GL_RGBA8, GL_RGBA, m_specification.width, m_specification.height, i);
                    break;
                
                case FramebufferTextureFormat::RED_INTEGER:
                    Utils::AttachColorTexture(m_colorAttachments[i], m_specification.samples, GL_R32I, GL_RED_INTEGER, m_specification.width, m_specification.height, i);
                    break;
            }
        }
    }

    if(true) {
        Utils::CreateTextures(multisample, &m_depthAttachment, 1);
        Utils::BindTexture(multisample, m_depthAttachment);
        switch(m_depthAttachmentSpecification.textureFormat)
        {
            case FramebufferTextureFormat::DEPTH24STENCIL8:
                Utils::AttachDepthTexture(m_depthAttachment, m_specification.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_specification.width, m_specification.height);
                break;
        }
    }

    if(m_colorAttachments.size() > 1)
    {
        EX_ASSERT_EXPR(m_colorAttachments.size() <= 4);
        GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(m_colorAttachments.size(), buffers);
    }
    else if(m_colorAttachments.empty())
    {
        glDrawBuffers(GL_NONE);
    }

    EX_ASSERT_EXPR(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_rendererId);
    glViewport(0, 0, m_specification.width, m_specification.height);
}

void OpenGLFramebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFramebuffer::Resize(Uint32 width, Uint32 height)
{
    if(width == 0 || height == 0 || width > MAX_FRAMEBUFFER_SIZE || height > MAX_FRAMEBUFFER_SIZE)
    {
        EX_LOG(Warning, LogCategory::Driver, "Attempted to resize framebuffer to {}, {}", width, height);
        return;
    }

    m_specification.width = width;
    m_specification.height = height;

    Invalidate();
}

Int32 OpenGLFramebuffer::ReadPixel(Uint32 attachmentIndex, Int32 x, Int32 y)
{
    EX_ASSERT_EXPR(attachmentIndex < m_colorAttachments.size());

    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    Int32 pixelData;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
    return pixelData;
}

void OpenGLFramebuffer::ClearAttachment(Uint32 attachmentIndex, Int32 value)
{
    EX_ASSERT_EXPR(attachmentIndex < m_colorAttachments.size());
    
    auto& spec = m_colorAttachmentSpecifications[attachmentIndex];
    glClearTexImage(m_colorAttachments[attachmentIndex], 0, Utils::EntixFBTextureFormatToGL(spec.textureFormat), GL_INT, &value);
}