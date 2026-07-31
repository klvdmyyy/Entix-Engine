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
        ENTIX_API static ResourceManager& Instance();

        template<std::derived_from<Resource> T>
        ResourceHandle<T> Load(const ResourceId& resourceId)
        {
            const auto resourceTypeIndex = std::type_index(typeid(T));
            auto& typeResources = m_resources[resourceTypeIndex];

            auto it = typeResources.find(resourceId);

            if(it != typeResources.end() && it->second.get())
            {
                m_refCounts[resourceId]++;
                return ResourceHandle<T>(resourceId, this);
            }

            typeResources[resourceId] = ThreadPool::Instance().Enqueue([&]() -> Ref<Resource> {
                auto resource = CreateRef<T>(resourceId);

                EX_LOG(Resources, Debug, "Loading resource: '{}'", resourceId.GetFilenameString());
                if(auto res = resource->Load(); res.IsError())
                {
                    EX_LOG(Resources, Error, "Failed to load resource '{}'. Error: {}", resourceId.GetFilenameString(), res.UnwrapErr());
                    return Ref<Resource>(nullptr);
                }

                m_refCounts[resourceId]++;
                return resource;
            });

            return ResourceHandle<T>(resourceId, this);
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

        ENTIX_API void* GetResource(std::type_index idx, const ResourceId& resourceId) const noexcept;
        ENTIX_API bool HasResource(std::type_index idx, const ResourceId& resourceId) const noexcept;

    private:
        template<typename T>
        using ResourceStorage =
            std::unordered_map<std::type_index, std::unordered_map<ResourceId, T, ResourceId::Hasher>>;

        ResourceStorage<std::shared_future<Ref<Resource>>> m_resources;
        std::unordered_map<ResourceId, Usize, ResourceId::Hasher> m_refCounts;
    };
}