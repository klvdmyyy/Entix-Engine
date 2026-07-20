// -*- mode: c++; -*-
#pragma once

#include "Core/Debug/Log.h"

#include "Resources/ResourceId.h"

#include <typeindex>

class Resource {
public:
    virtual ~Resource() = default;

    enum class State {
        Unloaded,
        Ready,
        Failed,
    };

    explicit Resource(const ResourceId& id)
        : m_id(id), m_state(State::Unloaded), m_refCount(0), m_memorySize(0)
    {
    }

    ResourceId GetId() const noexcept { return m_id; }
    State GetState() const noexcept { return m_state; }
    Bytes GetMemorySize() const noexcept { return m_memorySize; }

    void AddRef() { m_refCount++; }
    void Release() {
        if(--m_refCount == 0) {
            EX_LOG(Error, LogCategory::Resource, "TODO: Resource should be unloaded now!!! It's unimplemented functionality!");
            // UnloadInternal();
        }
    }
    Uint64 GetRefCount() const { return m_refCount; }

    std::type_index GetLoaderType() const noexcept { return m_loaderType; }
    std::type_index GetResourceType() const noexcept { return m_resourceType; }

protected:
    friend class ResourceManager;
    friend class ResourceLoader;

    void SetLoaderType(std::type_index loaderType) noexcept { m_loaderType = loaderType; }
    void SetResourceType(std::type_index resourceType) noexcept { m_resourceType = resourceType; }

    void SetMemorySize(Bytes memorySize) noexcept { m_memorySize = memorySize; }

    void SetState(State state) noexcept { m_state = state; }

    virtual void HotReload([[maybe_unused]] Scope<Resource> other)
    {
        EX_LOG(Warning, LogCategory::Resource, "Resource doesn't support hot-reload: '{}'", ((std::filesystem::path)(m_id)).filename().string());
    }

private:
    ResourceId m_id;
    State m_state;
    Uint64 m_refCount;
    Bytes m_memorySize;

    std::type_index m_loaderType = typeid(std::nullptr_t);
    std::type_index m_resourceType = typeid(std::nullptr_t);
};