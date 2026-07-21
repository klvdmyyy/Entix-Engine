#pragma once

#include "Core/Base.h"
#include "Core/Types.h"

#include <initializer_list>
#include <vector>

namespace Renderer {
    enum class FramebufferTextureFormat {
        RGBA8,
        RED_INTEGER,
        DEPTH24STENCIL8,
        
        // Defaults
        Depth = DEPTH24STENCIL8
    };

    struct FramebufferTextureSpecification {
        FramebufferTextureSpecification() = default;
        FramebufferTextureSpecification(FramebufferTextureFormat format) : textureFormat(format) {}

        FramebufferTextureFormat textureFormat = FramebufferTextureFormat::Depth;
    };

    struct FramebufferAttachmentSpecification {
        FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> attachments;
    };

    struct FramebufferSpecification {
        Uint32 width = 0, height = 0;
        FramebufferAttachmentSpecification attachmentSpec;
        Uint32 samples = 1;

        bool swapChainTarget = false;
    };

    class Framebuffer {
    public:
        virtual ~Framebuffer() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void Resize(Uint32 width, Uint32 height) = 0;
        virtual int ReadPixel(Uint32 attachmentIndex, Int32 x, Int32 y) = 0;

        virtual void ClearAttachment(Uint32 attachmentIndex, Int32 value) = 0;

        virtual Uint32 GetRendererId() const noexcept = 0;
        virtual Uint32 GetColorAttachmentRendererId(Uint32 index = 0) const = 0;

        virtual const FramebufferSpecification& GetSpecification() const noexcept = 0;
    };
}