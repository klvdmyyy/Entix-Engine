// -*- mode: c++; -*-
#pragma once

#include "Entix/Core/Types.h"
#include "Entix/Core/Assert.h"
#include "Entix/Core/IO/Stream.h"

#include "Entix/Resources/Resource.h"

#include <concepts>
#include <filesystem>

namespace Entix
{
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

        explicit operator bool() const noexcept {
            return IsValid();
        }

        T* operator->() const {
            EX_ASSERT_FMT(IsValid(), "Trying to use invalid resource handle!");
            return m_ptr;
        }

        T* operator*() const {
            EX_ASSERT_FMT(IsValid(), "Trying to use invalid resource handle!");
            return m_ptr;
        }

        [[nodiscard]]
        T* Get() const noexcept { return m_ptr; }

        [[nodiscard]]
        ResourceId GetId() const noexcept { return m_id; }

    private:
        ResourceId m_id;
        T* m_ptr;
    };
}