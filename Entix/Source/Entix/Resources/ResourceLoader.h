// -*- mode: c++; -*-
#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Memory.h"

#include "Entix/Resources/Resource.h"

#include <vector>
#include <typeindex>

namespace Entix
{
    class ResourceLoader {
    public:
        virtual ~ResourceLoader() = default;

        virtual Scope<Resource> Load(const ResourceId& id) = 0;

        inline const std::vector<ResourceId>& GetAllIds() const noexcept
        {
            return m_ids;
        }

    private:
        friend class ResourceManager;
        
        std::vector<ResourceId> m_ids;

        Scope<Resource> LoadInternal(const ResourceId& id);
    };
}
