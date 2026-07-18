#include "GameFramework/TextureLoader.h"

#include "Renderer/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <tracy/Tracy.hpp>

Scope<Resource> TextureLoader::Load(const ResourceId& id, IO::Reader& reader)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = nullptr;
    {
        ZoneScopedN("stbi_load_from_memory - TextureLoader");
        std::vector<stbi_uc> image(reader.Size());
        reader.Read(image.data(), reader.Size());
        data = stbi_load_from_memory(image.data(), image.size(), &width, &height, &channels, 0);
    }

    if(data)
    {
        Renderer::ImageFormat format;
        if(channels == 4) {
            format = Renderer::ImageFormat::RGBA8;
        }
        else if(channels == 3) {
            format = Renderer::ImageFormat::RGB8;
        }
        else {
            EX_LOG(Error, LogCategory::Resource, "Image channels count is {}. It's an error or just unsupported for now and cause errors.", channels);
            format = Renderer::ImageFormat::RGB8;
        }

        auto spec = Renderer::TextureSpecification{
            .width = static_cast<Uint32>(width),
            .height = static_cast<Uint32>(height),
            .format = format,
        };

        auto texture = m_rendererContext->CreateTexture(id, spec);

        texture->SetData(data, static_cast<Uint32>(width * height * channels));

        stbi_image_free(data);

        return Scope<Renderer::Texture>(texture);
    } else {
        EX_LOG(Error, LogCategory::Resource, "Failed to read texture image data!");
        return Scope<Renderer::Texture>(nullptr);
    }
}