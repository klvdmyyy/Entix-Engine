#include "Entix/Resources/Resource.h"

#include <tracy/Tracy.hpp>

namespace Entix
{
    Result<void> Resource::Load()
    {
        ZoneScopedN("Resource loading");
        ZoneTextF("%s", GetId().GetFilepathString().c_str());

        auto res = LoadInternal();
        m_loaded = res.IsSuccess();
        return res;
    }

    void Resource::Unload()
    {
        ZoneScopedN("Resource unloading");
        ZoneTextF("%s", GetId().GetFilepathString().c_str());

        if(!m_loaded) return;
        
        UnloadInternal();
        m_loaded = false;
    }
}