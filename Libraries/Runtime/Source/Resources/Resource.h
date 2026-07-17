// -*- mode: c++; -*-
#pragma once

#include "Core/Debug/Log.h"

#include "Resources/ResourceId.h"

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

    ResourceId GetId() const { return m_id; }
    State GetState() const { return m_state; }
    size_t GetMemorySize() const { return m_memorySize; }

    void AddRef() { m_refCount++; }
    void Release() {
        if(--m_refCount == 0) {
            EX_LOG(Error, LogCategory::Resource, "TODO: Resource should be unloaded now!!! It's unimplemented functionality!");
            // UnloadInternal();
        }
    }
    Uint64 GetRefCount() const { return m_refCount; }

protected:
    friend class ResourceManager;
    friend class ResourceLoader;

    void SetState(State state) { m_state = state; }

//     virtual bool LoadFromStream(IO::Reader& reader) = 0;
//     virtual bool Unload() = 0;

//     bool UnloadInternal() {
//       if (m_refCount != 0) {
//         EX_LOG(Warning,
//                R"(Unloading resource which is currently in usage!
// ResourceId: {}
// Reference Count: {}
// Memory Size: {})",
//                m_id, m_refCount, m_memorySize);
//       }
//       EX_LOG(Trace, "Unloading resource: {}", m_id);
//         m_state = State::Unloaded;
//       return Unload();
//     }

private:
    ResourceId m_id;
    State m_state;
    Uint64 m_refCount;
    size_t m_memorySize;
};