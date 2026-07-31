#include "Entix/WSI/Base.h"

#include "Entix/Core/Events/Dispatcher.h"

#include <SDL3/SDL.h>

#include <SDL3/SDL_vulkan.h>

namespace Entix::WSI
{
    Result<void> Initialize()
    {
        if(!SDL_Init(SDL_INIT_VIDEO))
            return Error(SDL_GetError());

        if(!SDL_Vulkan_LoadLibrary(nullptr))
            return Error(SDL_GetError());

        return {};
    }

    void Shutdown()
    {
        SDL_Vulkan_UnloadLibrary();
        SDL_Quit();
    }

    void PollEvents()
    {
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    EventBus::PublishEvent(WindowCloseEvent(event.window.windowID));
                    break;

                case SDL_EVENT_WINDOW_RESIZED:
                    EventBus::PublishEvent(WindowResizeEvent(
                        event.window.windowID,
                        static_cast<Uint32>(event.window.data1),
                        static_cast<Uint32>(event.window.data2)
                    ));
                    break;

                case SDL_EVENT_MOUSE_MOTION:
                    EventBus::PublishEvent(MouseMotionEvent(
                        event.window.windowID,
                        event.motion.x,
                        event.motion.y,
                        event.motion.xrel,
                        event.motion.yrel
                    ));
                    break;

                default:
                    break;
            }
        }
    }

    Result<std::vector<const char*>> GetRequiredVulkanInstanceExtensions()
    {
        Uint32 count;
        auto extensions = SDL_Vulkan_GetInstanceExtensions(&count);

        if(extensions == nullptr)
            return Error(SDL_GetError());

        return std::vector<const char*>(extensions, extensions + count);
    }
}
