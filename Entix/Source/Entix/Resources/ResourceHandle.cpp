#include "Entix/Resources/ResourceHandle.h"
#include "Entix/Resources/ResourceManager.h"

namespace Entix
{
    bool HasResourceByIndex(ResourceManager *rm, std::type_index idx, const ResourceId& id)
    {
        return rm->HasResource(idx, id);
    }

    void* GetResourceByIndex(ResourceManager *rm, std::type_index idx, const ResourceId &id)
    {
        return rm->GetResource(idx, id);
    }

    void DecrementResourceRefCountByIndex(
        [[maybe_unused]] ResourceManager* rm,
        [[maybe_unused]] const ResourceId& id
    )
    {
        // rm->DecrementRefCountOf(id);
    }
}