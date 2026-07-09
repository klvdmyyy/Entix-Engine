// -*- mode: c++; -*-
#pragma once

#include "Core/Types.h"
#include "Core/IO/Base.h"

#include <type_traits>
#include <filesystem>

class ResourceId {
    size_t m_hash;
    bool m_null;

public:
    ResourceId(std::nullptr_t _)
        : m_hash(0), m_null(true);
    {
    }
    
    explicit ResourceId(const std::filesystem::path& path)
        : m_hash(std::hash<std::filesystem::path>{}(path)), m_null(false)
    {
    }

    size_t Hash() const { return m_hash; }
    
    operator bool() const { return !m_null; }
    
    bool operator==(const ResourceId& other) const
    {
        return other.m_hash == m_hash && other.m_null == other.m_null;
    }

    struct Hasher {
        size_t operator() (const ResourceId& id) const
        {
            return id.Hash();
        }
    };
};

class Resource {
public:
    virtual ~Resource() = default;

    enum class State {
        Unloaded,
        Loading,
        Ready,
        Failed,
    };

    explicit Resource(const ResourceId& id)
        : m_id(id), m_state(State::Unloaded), m_refCount(0), m_memorySize(0)
    {
    }

    ResourceId GetId() const { return m_id; }
    State GetState() const { return m_state; }
    size_t GetMemorySize() const { return m_memorySize; }

    void AddRef() { m_refCount++; }
    void Release() {
        if(--m_refCount == 0) {
            Unload();
        }
    }
    Uint64 GetRefCount() const { return m_refCount; }

protected:
    friend class ResourceManager;

    void SetState(State state) { m_state = state; }

    virtual bool LoadFromStream(IO::Reader& reader) = 0;
    virtual bool Unload() = 0;

private:
    ResourceId m_id;
    State m_state;
    Uint64 m_refCount;
    size_t m_memorySize;
};

template<std::derived_from<Resource> T>
class ResourceHandle {
public:
    ResourceHandle()
        : m_id(nullptr), m_ptr(nullptr)
    {
    }

    explicit ResourceHandle(const ResourceId& id, T* ptr = nullptr)
        : m_id(id), m_ptr(ptr)
    {
    }

    ~ResourceHandle()
    {
        m_ptr->Release();
    }

    bool IsValid() const { return m_ptr != nullptr && m_ptr->GetState() == Resource::State::Ready; }
    bool IsLoading() const { return m_ptr != nullptr && m_ptr->GetState() == Resource::State::Loading; }

    T* operator->() const {
        EX_ASSERT(IsValid(), "Trying to use invalid resource handle!");
        return m_ptr;
    }

    T* operator*() const {
        EX_ASSERT(IsValid(), "Trying to use invalid resource handle!");
        return m_ptr;
    }

    T* Get() const { return m_ptr; }

    ResourceId GetId() const { return m_id; }

private:
    ResourceId m_id;
    T* m_ptr;
};