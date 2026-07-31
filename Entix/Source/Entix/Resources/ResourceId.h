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

        ResourceId(const ResourceId&) = default;
        ResourceId operator=(const ResourceId& other)
        {
            return ResourceId((std::filesystem::path)other);
        }

        [[nodiscard]]
        ENTIX_API Usize Hash() const noexcept;
        
        operator bool() const { return !m_null; }

        [[nodiscard]] const std::filesystem::path& GetFilepath() const noexcept { return k_filepath; }
        [[nodiscard]] String GetFilepathString() const noexcept { return GetFilepath().string(); }

        [[nodiscard]] std::filesystem::path GetFilename() const noexcept { return k_filepath.filename(); }
        [[nodiscard]] String GetFilenameString() const noexcept { return GetFilename().string(); }

        [[nodiscard]] std::filesystem::path GetExtension() const noexcept { return k_filepath.extension(); }
        [[nodiscard]] String GetExtensionString() const noexcept { return GetExtension().string(); }

        explicit operator Usize() const noexcept { return Hash(); }
        explicit operator std::filesystem::path() const noexcept { return GetFilepath(); }
        explicit operator String() const noexcept { return GetFilepathString(); }

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