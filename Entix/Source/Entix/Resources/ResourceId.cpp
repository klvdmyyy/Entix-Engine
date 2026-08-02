#include "Entix/Resources/ResourceId.h"

namespace Entix
{
    ResourceId::ResourceId()
            : k_filepath(), m_hash(0), m_null(true)
    {
    }
    
    ResourceId::ResourceId(const std::filesystem::path& path)
        : k_filepath(path), m_hash(std::hash<std::filesystem::path>{}(path)), m_null(false)
    {
    }

    Usize ResourceId::Hash() const noexcept { return m_hash; }
}