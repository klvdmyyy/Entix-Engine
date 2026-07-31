#include "Entix/Resources/ResourceManager.h"

namespace Entix
{
    ResourceManager& ResourceManager::Instance()
    {
        static ResourceManager s_instance;
        return s_instance;
    }
    
    void* ResourceManager::GetResource(std::type_index idx, const ResourceId& resourceId) const noexcept
    {
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
        return GetResource(idx, resourceId) == nullptr ? false : true;
    }
}