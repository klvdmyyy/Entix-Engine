#include "GameFramework/ShaderLoader.h"

#include <tracy/Tracy.hpp>

Scope<Resource> ShaderLoader::Load(const ResourceId& id, IO::Reader &reader)
{
    ZoneScoped;
    
    auto shader = Scope<Renderer::Shader>(m_rendererContext->CreateShader(id));

    shader->CompileFromStream(reader);

    return std::move(shader);
}