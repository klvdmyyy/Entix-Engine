#include "Entix/Resources/ResourceId.h"

#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"

#include <filesystem>

namespace Entix
{
    ResourceId::ResourceId(std::source_location location)
            : m_filepath(), m_hash(0), m_null(true)
    {
        EX_LOG(Resources, Debug, "Created zeroed resource id at `{}({}:{})`",
            std::filesystem::path(location.file_name()).filename().string(),
            location.line(), location.column());
    }
    
    ResourceId::ResourceId(const std::filesystem::path& path)
        : m_filepath(path), m_hash(std::hash<std::filesystem::path>{}(path)), m_null(false)
    {
    }

    Usize ResourceId::Hash() const noexcept { return m_hash; }
}