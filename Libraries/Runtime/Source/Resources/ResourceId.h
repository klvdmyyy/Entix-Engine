// -*- mode: c++; -*-
#pragma once

#include "Core/Types.h"
#include "Core/Base.h"

#include <filesystem>

namespace {
    template<class Key>
    struct std::hash;
}

class ResourceId {
    const std::filesystem::path k_filepath;

    size_t m_hash;
    bool m_null;

public:
    ResourceId()
        : k_filepath(), m_hash(0), m_null(true)
    {
    }
    
    explicit ResourceId(const std::filesystem::path& path)
        : k_filepath(path), m_hash(std::hash<std::filesystem::path>{}(path)), m_null(false)
    {
    }

    size_t Hash() const { return m_hash; }
    
    operator bool() const { return !m_null; }
    operator std::filesystem::path() const noexcept { return k_filepath; }
    operator String() const noexcept { return k_filepath.string(); }

    ResourceId operator=(const ResourceId& other)
    {
        return ResourceId((std::filesystem::path)other);
    }

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
