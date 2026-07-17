#include "GameFramework/ShaderLoader.h"

Scope<Resource> ShaderLoader::Load(const ResourceId& id, IO::Reader &reader)
{
    auto shader = Scope<Renderer::Shader>(m_rendererContext->CreateShader(id));

    shader->CompileFromStream(reader);

    return std::move(shader);
}