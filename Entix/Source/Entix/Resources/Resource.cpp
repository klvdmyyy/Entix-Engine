#include "Entix/Resources/Resource.h"

#include <tracy/Tracy.hpp>

namespace Entix
{
    Result<void> Resource::Load()
    {
        ZoneScopedN("Resource loading");
        ZoneTextF("%s", GetResourceId().GetFilepathString().c_str());

        auto res = LoadInternal();
        m_loaded = res.IsSuccess();
        return res;
    }

    Result<void> Resource::Reload()
    {
        ZoneScopedN("Resource reloading");
        ZoneTextF("%s", GetResourceId().GetFilepathString().c_str());

        auto res = ReloadInternal();
        m_loaded = res.IsSuccess();
        return res;
    }

    void Resource::Unload()
    {
        ZoneScopedN("Resource unloading");
        ZoneTextF("%s", GetResourceId().GetFilepathString().c_str());

        if(!m_loaded) return;
        
        UnloadInternal();
        m_loaded = false;
    }
}