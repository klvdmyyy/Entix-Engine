// -*- mode: c++; -*-
#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Types.h"
#include "Entix/Core/String.h"

#include <source_location>

#include <filesystem>

namespace std {
    template<class Key>
    struct hash;
}

namespace Entix
{
    class ResourceId {
        std::filesystem::path m_filepath;

        Usize m_hash;
        bool m_null;

    public:
        ENTIX_API ResourceId(std::source_location location = std::source_location::current());
        ENTIX_API explicit ResourceId(const std::filesystem::path& path);

        ResourceId(const ResourceId& other)
            : m_filepath(other.m_filepath),
              m_hash(other.m_hash),
              m_null(other.m_null)
        {
        }
        
        void operator=(const ResourceId& other)
        {
            if(this == &other)
                return;

            m_filepath = other.m_filepath;
            m_hash = other.m_hash;
            m_null = other.m_null;
        }

        [[nodiscard]]
        ENTIX_API Usize Hash() const noexcept;
        
        operator bool() const { return !m_null; }

        [[nodiscard]] const std::filesystem::path& GetFilepath() const noexcept { return m_filepath; }
        [[nodiscard]] String GetFilepathString() const noexcept { return GetFilepath().string(); }

        [[nodiscard]] std::filesystem::path GetFilename() const noexcept { return m_filepath.filename(); }
        [[nodiscard]] String GetFilenameString() const noexcept { return GetFilename().string(); }

        [[nodiscard]] std::filesystem::path GetExtension() const noexcept { return m_filepath.extension(); }
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