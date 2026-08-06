#pragma once

#include "Entix/Core/Memory.h"
#include "Entix/Core/Types.h"
#include "Entix/Core/Globals.h"

#include "Entix/Core/Debug/Logger.h"
#include "Entix/Core/Tasks/ThreadPool.h"

#include "Entix/Resources/ResourceId.h"
#include "Entix/Resources/ResourceHandle.h"
#include "Entix/Resources/Resource.h"

#include <future>
#include <typeindex>
#include <unordered_map>

namespace Entix
{
    class ResourceManager
    {
    public:
        template<typename T>
        using Future = std::shared_future<T>;

        ENTIX_API static void SetInstance(ResourceManager* rm);
        ENTIX_API static ResourceManager* Instance();

        virtual ~ResourceManager() = default;

        // Unable to copy
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;

        // Unable to move
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;

        template<std::derived_from<Resource> T, typename... Args>
        ResourceHandle<T> Load(const ResourceId& resourceId, Args&&... args)
        {
            const auto resourceTypeIndex = std::type_index(typeid(T));
            auto& typeResources = m_resources[resourceTypeIndex];

            auto it = typeResources.find(resourceId);

            if(it != typeResources.end() && it->second.get())
            {
                m_refCounts[resourceId]++;
                return ResourceHandle<T>(resourceId, this);
            }

            typeResources[resourceId] = ThreadPool::Instance().Enqueue([&, resourceId]() -> Ref<Resource> {
                auto resource = CreateRef<T>(resourceId, std::forward<Args>(args)...);

                EX_LOG(Resources, Debug, "Loading resource: '{}'", resourceId.GetFilenameString());
                if(auto res = resource->Load(); res.IsError())
                {
                    EX_LOG(Resources, Error, "Failed to load resource '{}'. Error: {}", resourceId.GetFilenameString(), res.UnwrapErr());
                    return Ref<Resource>(nullptr);
                }

                {
                    m_refCounts[resourceId]++;
                }

                return resource;
            });

            LoadMiddleware(resourceId);

            return ResourceHandle<T>(resourceId, this);
        }

        template<std::derived_from<Resource> T>
        void UnloadType()
        {
            auto typeResources = m_resources[std::type_index(typeid(T))];
            
            for(auto& [resourceId, resource] : typeResources)
            {
                (void)resourceId;
                resource.get()->Unload();
            }

            m_resources.erase(std::type_index(typeid(T)));
        }

        template<std::derived_from<Resource> T>
        T* GetResource(const ResourceId& resourceId) const noexcept
        {
            return static_cast<T*>(GetResource(std::type_index(typeid(T)), resourceId));
        }

        template<std::derived_from<Resource> T>
        bool HasResource(const ResourceId& resourceId) const noexcept
        {
            return HasResource(std::type_index(typeid(T)), resourceId);
        }

    protected:
        ResourceManager() = default;

        virtual void LoadMiddleware([[maybe_unused]] const ResourceId& resourceId) {}

        Result<Future<Ref<Resource>>> FindResourceById(const ResourceId& resourceId);

    private:
        friend bool HasResourceByIndex(
            ResourceManager* rm,
            std::type_index idx,
            const ResourceId& id
        );

        friend void* GetResourceByIndex(
            ResourceManager* rm,
            std::type_index idx,
            const ResourceId& id
        );

        friend void DecrementResourceRefCountByIndex(
            ResourceManager* rm,
            const ResourceId& id
        );

        ENTIX_API void* GetResource(std::type_index idx, const ResourceId& resourceId) const noexcept;
        ENTIX_API bool HasResource(std::type_index idx, const ResourceId& resourceId) const noexcept;

        ENTIX_API void DecrementRefCountOf(const ResourceId& resourceId);

        static Scope<ResourceManager> s_resourceManager;

        ENTIX_API static Scope<ResourceManager> CreateScopeRM();

        using StoragedResource = Future<Ref<Resource>>;

        template<typename T>
        using ResourceStorageInner =
            std::unordered_map<ResourceId, T, ResourceId::Hasher>;

        template<typename T>
        using ResourceStorage =
            std::unordered_map<std::type_index, ResourceStorageInner<T>>;

        ResourceStorage<StoragedResource> m_resources;
        std::unordered_map<ResourceId, Usize, ResourceId::Hasher> m_refCounts;
    };
}