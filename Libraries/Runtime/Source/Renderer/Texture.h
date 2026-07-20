#pragma once

#include "Core/Base.h"
#include "Core/IO/Base.h"

#include "Resources/Resource.h"

namespace Renderer {
    enum class ImageFormat {
        R8,
        RGB8,
        RGBA8,
        RGBA32F
    };

    struct TextureSpecification {
        Uint32 width;
        Uint32 height;
        ImageFormat format = ImageFormat::RGBA8;
        bool generateMips = true;
    };

    class Texture : public Resource {
    public:
        Texture(const ResourceId& id) : Resource(id) {}
        virtual ~Texture() = default;

        virtual const TextureSpecification& GetSpecification() const noexcept = 0;

        virtual Uint32 GetWidth() const noexcept = 0;
        virtual Uint32 GetHeight() const noexcept = 0;

        virtual void SetData(void* data, Uint32 size) = 0;

        virtual void Bind(Uint32 slot = 0) const = 0;

        virtual Uint32 GetRendererId() const noexcept = 0;

        virtual bool operator==(const Texture& other) const noexcept = 0;
    };
}