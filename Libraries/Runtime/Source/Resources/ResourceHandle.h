// -*- mode: c++; -*-
#pragma once

#include "Core/Types.h"
#include "Core/Assert.h"
#include "Core/IO/Base.h"

#include "Resources/Resource.h"

#include <concepts>
#include <filesystem>

template<std::derived_from<Resource> T>
class ResourceHandle {
public:
    ResourceHandle()
        : m_id(), m_ptr(nullptr)
    {
    }

    explicit ResourceHandle(const ResourceId& id)
        : m_id(id), m_ptr(nullptr)
    {
    }

    explicit ResourceHandle(const ResourceId& id, T* ptr = nullptr)
        : m_id(id), m_ptr(ptr)
    {
    }

    ~ResourceHandle()
    {
        if(m_ptr)
            m_ptr->Release();
    }

    bool IsValid() const noexcept { return m_ptr != nullptr && m_ptr->GetState() == Resource::State::Ready; }
    bool IsLoading() const noexcept { return m_ptr != nullptr && m_ptr->GetState() == Resource::State::Loading; }

    operator bool() const noexcept {
        return IsValid();
    }

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