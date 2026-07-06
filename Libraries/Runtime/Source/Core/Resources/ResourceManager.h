// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/Memory.h"
#include "Core/String.h"

#include "Renderer/Shader.h"
#include "Renderer/Context.h"

#include <unordered_map>
#include <type_traits>

class ResourceManager {
public:
    static ResourceManager& Instance();

    FORCE_INLINE
    inline void SetRendererContext(const Ref<Renderer::Context>& rendererContext)
    {
        m_rendererContext = rendererContext;
    }

    const Ref<Renderer::Shader>& LoadShader(const String& filepath);
    const Ref<Renderer::Shader>& GetShader(const String& filepath) const;
    void ReloadShader(const String& filepath);

private:
    ResourceManager();

    Ref<Renderer::Context> m_rendererContext;

    std::unordered_map<String, Ref<Renderer::Shader>> m_shadersMap;
};