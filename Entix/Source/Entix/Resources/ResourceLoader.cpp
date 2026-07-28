#include "Entix/Resources/ResourceLoader.h"

namespace Entix
{
    Scope<Resource> ResourceLoader::LoadInternal(const ResourceId& id)
    {
        if(std::find(m_ids.begin(), m_ids.end(), id) == m_ids.end())
            m_ids.push_back(id);

        return Load(id);
    }
}