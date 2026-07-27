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
        
        // if(!SDL_Vulkan_LoadLibrary(nullptr))
        //     return Error(SDL_GetError());

        return {};
    }

    void Shutdown()
    {
        // SDL_Vulkan_UnloadLibrary();
        SDL_Quit();
    }

    void PollEvents()
    {
        auto& bus = EventBus::Instance();

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    bus.Send<WindowCloseEvent>(WindowCloseEvent {
                        .id = event.window.windowID
                    });
                    break;
                
                case SDL_EVENT_WINDOW_RESIZED:
                    bus.Send<WindowResizeEvent>(WindowResizeEvent {
                        .id = event.window.windowID,
                        .width = static_cast<Uint32>(event.window.data1),
                        .height = static_cast<Uint32>(event.window.data2)
                    });
                    break;
                
                case SDL_EVENT_MOUSE_MOTION:
                    bus.Send<MouseMotionEvent>(MouseMotionEvent {
                        .windowId = event.motion.windowID,
                        .xPosition = event.motion.x,
                        .yPosition = event.motion.y,
                        .xRelative = event.motion.xrel,
                        .yRelative = event.motion.yrel
                    });
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