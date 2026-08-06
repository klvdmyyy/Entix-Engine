#include "Entix/Resources/ResourceManager.h"

#include <algorithm>
#include <ranges>

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

        EX_LOG(Resources, Error, "Trying to access to unexisting resource.\nType: {}\nId: {}", idx.name(), (bool)resourceId ? resourceId.GetFilenameString() : "null");
        return nullptr;
    }

    bool ResourceManager::HasResource(std::type_index idx, const ResourceId& resourceId) const noexcept
    {
        ZoneScoped;
        ZoneTextF("%s", resourceId.GetFilepathString().c_str());

        return GetResource(idx, resourceId) == nullptr ? false : true;
    }

    Result<ResourceManager::Future<Ref<Resource>>> ResourceManager::FindResourceById(const ResourceId& resourceId)
    {
        auto typeIt = std::ranges::find_if(m_resources, [&resourceId](const auto& pair){
            return std::ranges::any_of(pair.second, [&resourceId](const auto& pair) {
                return pair.second.get()->GetResourceId() == resourceId;
            });
        });

        if(typeIt == m_resources.end())
        {
            return Error(std::format("Can't find type index for resource: {}", resourceId.GetFilenameString()));
        }

        auto resourceIt = std::ranges::find_if(typeIt->second, [&resourceId](const auto& pair) {
            return pair.second.get()->GetResourceId() == resourceId;
        });

        if(resourceIt == typeIt->second.end())
        {
            return Error(std::format("Can't find resource: {}", resourceId.GetFilenameString()));
        }

        return resourceIt->second;
    }

    void ResourceManager::DecrementRefCountOf(const ResourceId& resourceId)
    {
        (void)resourceId;
        // if(m_refCounts[resourceId] == 0)
        //     Panic(std::format("Unreachable. Resource Id: {}", resourceId.GetFilepathString()));

        // if(--m_refCounts[resourceId] == 0)
        // {
        //     EX_LOG(Resources, Info, "Ref counting of '{}' is reached zero!", resourceId.GetFilenameString());
        // }
    }
}