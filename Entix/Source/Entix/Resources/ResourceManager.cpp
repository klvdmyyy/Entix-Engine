#include "Entix/Resources/ResourceManager.h"

#include <tracy/Tracy.hpp>

namespace Entix
{
    Scope<ResourceManager> ResourceManager::s_resourceManager = ResourceManager::CreateScopeRM();

    Scope<ResourceManager> ResourceManager::CreateScopeRM()
    {
        return Scope<ResourceManager>(new ResourceManager());
    }

    void ResourceManager::SetInstance(ResourceManager* rm)
    {
        s_resourceManager.reset(rm);
    }

    ResourceManager* ResourceManager::Instance()
    {
        return s_resourceManager.get();
    }

    void* ResourceManager::GetResource(std::type_index idx, const ResourceId& resourceId) const noexcept
    {
        ZoneScoped;
        ZoneTextF("%s", resourceId.GetFilepathString().c_str());

        auto& typeResources = m_resources.at(idx);
        auto it = typeResources.find(resourceId);

        if(it != typeResources.end())
        {
            return static_cast<void*>(it->second.get().get());
        }

        return nullptr;
    }

    bool ResourceManager::HasResource(std::type_index idx, const ResourceId& resourceId) const noexcept
    {
        ZoneScoped;
        ZoneTextF("%s", resourceId.GetFilepathString().c_str());

        return GetResource(idx, resourceId) == nullptr ? false : true;
    }
}