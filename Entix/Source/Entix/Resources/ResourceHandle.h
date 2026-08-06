#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Resources/Resource.h"
#include "Entix/Resources/ResourceId.h"

#include <concepts>
#include <source_location>
#include <typeindex>

namespace Entix
{
    class ResourceManager;

    ENTIX_API bool  HasResourceByIndex(
        ResourceManager* rm,
        std::type_index idx,
        const ResourceId& id
    );
    ENTIX_API void* GetResourceByIndex(
        ResourceManager* rm,
        std::type_index idx,
        const ResourceId& id
    );
    ENTIX_API void DecrementResourceRefCountByIndex(
        ResourceManager* rm,
        const ResourceId& id
    );

    template<std::derived_from<Resource> T>
    class ResourceHandle
    {
    public:
        ResourceHandle(std::source_location location = std::source_location::current()) : m_resourceId(location), m_resourceManager(nullptr) {}
        ResourceHandle(ResourceId id, ResourceManager* rm)
            : m_resourceId(id), m_resourceManager(rm) {}
        
        ~ResourceHandle()
        {
            DecrementResourceRefCountByIndex(m_resourceManager, m_resourceId);
        }
        
        T* Get() const noexcept
        {
            return static_cast<T*>(GetResourceByIndex(
                m_resourceManager,
                std::type_index(typeid(T)),
                m_resourceId
            ));
        }

        bool IsValid() const noexcept
        {
            return m_resourceManager && HasResourceByIndex(m_resourceManager, std::type_index(typeid(T)), m_resourceId);
        }

        const ResourceId& GetId() const noexcept
        {
            return m_resourceId;
        }

        ResourceHandle(const ResourceHandle<T>& other)
            : m_resourceId(other.m_resourceId),
              m_resourceManager(other.m_resourceManager)
        {
        }

        void operator=(const ResourceHandle<T>& other)
        {
            if(this == &other)
                return;

            m_resourceId = other.m_resourceId;
            m_resourceManager = other.m_resourceManager;
        }

        T* operator->() const
        {
            return Get();
        }

        T& operator*() const
        {
            return *Get();
        }

        // Conversion from derived to base or from base to derived types in ResourceHandle.
        //
        // Necessary for conversions like:
        // * ResourceHandle<VulkanShader>  <-> ResourceHandle<RHI::Shader>
        // * ResourceHandle<VulkanTexture> <-> ResourceHandle<RHI::Texture>
        // * and e.t.c.
        template<typename N>
            requires (std::derived_from<T, N>
                   || std::derived_from<N, T>
                   || std::is_convertible_v<T, N>
                   || std::is_convertible_v<T*, N*>)
        operator ResourceHandle<N>()
        {
            return ResourceHandle<N>(m_resourceId, m_resourceManager);
        }

        operator bool() const noexcept
        {
            return IsValid();
        }

    private:
        ResourceId m_resourceId;
        ResourceManager* m_resourceManager;
    };
}