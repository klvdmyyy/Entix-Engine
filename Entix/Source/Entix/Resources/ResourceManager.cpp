#include "Entix/Resources/ResourceManager.h"

#include "Entix/Core/Events/Dispatcher.h"

#include <algorithm>

#include <tracy/Tracy.hpp>

namespace Entix
{
    ResourceManager::ResourceManager(ThreadPool& threadPool, bool enableHotReload)
        : m_threadPool(threadPool), k_enableHotReload(enableHotReload)
    {
        EX_LOG(Resources, Info, "Initializing ResourceManager. Hot reload are {}", enableHotReload ? "enabled" : "disabled");
        if(k_enableHotReload)
        {
            m_fileWatcher = CreateScope<FileWatcher>(threadPool, [this](const std::filesystem::path& path)
            {
                ReloadResource(ResourceId(path));
            });
        }
    }

    void ResourceManager::ReloadResource(const ResourceId& resourceId)
    {
        ZoneScopedN("Resource reloading");
        ZoneTextF("%s", resourceId.GetFilepathString().c_str());

        if(auto resourceResult = FindResourceById(resourceId); resourceResult.IsSuccess())
        {
            EX_LOG(Resources, Info, "Changes detected. Reloading resource: '{}'", resourceId.GetFilenameString());

            Ref<Resource> resource = resourceResult.Unwrap().get();

            if(auto reloadResult = resource->Reload(); reloadResult.IsError())
            {
                EX_LOG(Resources, Error, "Failed to reload resource '{}':\n{}\nDependencies wouldn't be signaled about resource reloading to keep older version!", resourceId.GetFilenameString(), reloadResult.UnwrapErr());
                return;
            }

            EventBus::PublishEvent(ResourceReloadedEvent(resourceId));
        }
        else
        {
            EX_LOG(Resources, Error, "Changes detected in resource '{}' but it won't be found. It's must be unreachable error! Error: {}", resourceId.GetFilenameString(), resourceResult.UnwrapErr());
        }
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
