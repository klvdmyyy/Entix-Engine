// -*- mode: c++; -*-
#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Types.h"
#include "Entix/Core/String.h"

#include <filesystem>

namespace std {
    template<class Key>
    struct hash;
}

namespace Entix
{
    class ResourceId {
        const std::filesystem::path k_filepath;

        Usize m_hash;
        bool m_null;

    public:
        ENTIX_API ResourceId();
        ENTIX_API explicit ResourceId(const std::filesystem::path& path);

        [[nodiscard]]
        ENTIX_API Usize Hash() const noexcept;
        
        operator bool() const { return !m_null; }
        operator std::filesystem::path() const noexcept { return k_filepath; }
        operator String() const noexcept { return k_filepath.string(); }

        ResourceId(const ResourceId&) = default;
        ResourceId operator=(const ResourceId& other)
        {
            return ResourceId((std::filesystem::path)other);
        }

        bool operator==(const ResourceId& other) const
        {
            return other.m_hash == m_hash && other.m_null == m_null;
        }

        struct Hasher {
            Usize operator() (const ResourceId& id) const
            {
                return id.Hash();
            }
        };
    };
}