#include "Entix/Resources/Resource.h"

namespace Entix
{
    Resource::Resource(const ResourceId& id)
        : m_id(id), m_state(State::Unloaded), m_refCount(0), m_memorySize(0)
    {}

    void Resource::Release()
    {
        if (--m_refCount == 0)
        {
            EX_LOG(Resources, Error,
                    "TODO: Resource should be unloaded now!!! It's "
                    "unimplemented functionality!");
            // UnloadInternal();
        }
    }
}