// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/Memory.h"

#include "Core/IO/Base.h"

#include "Resources/Resource.h"

#include <vector>
#include <typeindex>

class ResourceLoader {
public:
    virtual ~ResourceLoader() = default;

    virtual Scope<Resource> Load(const ResourceId& id, IO::Reader& reader) = 0;

    inline const std::vector<ResourceId>& GetAllIds() const noexcept
    {
        return m_ids;
    }

private:
    friend class ResourceManager;
    
    std::vector<ResourceId> m_ids;

    FORCE_INLINE
    inline Scope<Resource> LoadInternal(IO::Reader& reader, const ResourceId& id)
    {
        if(std::find(m_ids.begin(), m_ids.end(), id) == m_ids.end())
            m_ids.push_back(id);

        return Load(id, reader);
    }
};
