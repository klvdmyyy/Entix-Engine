#include "GameFramework/ShaderLoader.h"

#include <tracy/Tracy.hpp>

Scope<Resource> ShaderLoader::Load(const ResourceId& id, IO::Reader &reader)
{
    ZoneScoped;
    
    Renderer::ShaderSpecification spec{};
    auto shader = Scope<Renderer::Shader>(m_rendererContext->CreateShader(id, spec));

    shader->CompileFromStream(reader);

    return std::move(shader);
}