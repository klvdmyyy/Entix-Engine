#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Types.h"
#include "Entix/Core/Events/Event.h"

#include "Entix/Core/Result.h"

#include <vector>

namespace Entix
{
    EX_DEFINE_ID_TYPE(WindowId);
    EX_DEFINE_EVENT_CATEGORY(WindowEventCategory);
    EX_DEFINE_EVENT_CATEGORY(MouseInputEventCategory);

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

    struct MouseMotionEvent
    {
        using Category = MouseInputEventCategory;

        WindowId windowId;

        float xPosition;
        float yPosition;

        float xRelative;
        float yRelative;
    };

    namespace WSI
    {
        ENTIX_API Result<void> Initialize();
        ENTIX_API void Shutdown();

        ENTIX_API void PollEvents();

        /** Vulkan things */
        
        ENTIX_API Result<std::vector<const char*>> GetRequiredVulkanInstanceExtensions();
    }
}