#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Resources/Resource.h"
#include "Entix/Resources/ResourceId.h"

#include <concepts>
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

    template<std::derived_from<Resource> T>
    class ResourceHandle
    {
    public:
        ResourceHandle() : m_resourceManager(nullptr) {}
        ResourceHandle(ResourceId id, ResourceManager* rm)
            : m_resourceId(id), m_resourceManager(rm) {}
        
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

        T* operator->() const
        {
            return Get();
        }

        T& operator*() const
        {
            return *Get();
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