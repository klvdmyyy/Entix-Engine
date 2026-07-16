#include "Core/Resources/ResourceManager.h"

#include "Core/StringCommandRunner.h"

#include "Core/Assert.h"
#include "Core/Debug/Log.h"

#include "Core/IO/FileReader.h"

ResourceManager::ResourceManager()
{
}

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager s_instance;
    return s_instance;
}

const Ref<Renderer::Shader>& ResourceManager::LoadShader(const String& filepath)
{
    auto searchIt = m_shadersMap.find(filepath);

    if(searchIt != m_shadersMap.end()) {
        return searchIt->second;
    }

    auto shaderReader = CreateScope<IO::FileReader>(filepath);

    auto loaded = Ref<Renderer::Shader>(m_rendererContext->CreateShader(*shaderReader));

    m_shadersMap.insert({filepath, std::move(loaded)});

    return m_shadersMap.at(filepath);
}

const Ref<Renderer::Shader>& ResourceManager::GetShader(const String& filepath) const
{
    return m_shadersMap.at(filepath);
}

void ResourceManager::ReloadShader(const String& filepath)
{
    auto searchIt = m_shadersMap.find(filepath);

    if(searchIt == m_shadersMap.end()) {
        return;
    }

    auto shaderReader = CreateScope<IO::FileReader>(filepath);
    searchIt->second.reset(m_rendererContext->CreateShader(*shaderReader));
}

