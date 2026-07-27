#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Types.h"
#include "Entix/Core/Events/Event.h"

namespace Entix
{
    EX_DEFINE_ID_TYPE(WindowId);
    EX_DEFINE_EVENT_CATEGORY(WindowEventCategory);

    struct WindowCloseEvent
    {
        using Category = WindowEventCategory;

        WindowId id;
    };

    struct WindowResizeEvent
    {
        using Category = WindowEventCategory;

        WindowId id;
        Uint32 width;
        Uint32 height;
    };

    namespace WSI
    {
        ENTIX_API bool PollEvents();
    }
}