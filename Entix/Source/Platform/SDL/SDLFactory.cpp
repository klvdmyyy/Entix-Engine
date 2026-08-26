#include "Platform/SDL/SDLFactory.h"

#include "Entix/Core/Panic.h"
#include "Entix/Core/Events/Dispatcher.h"

#include "Entix/WSI/Events.h"

#include "Platform/SDL/WindowSDL.h"

#include "Platform/Vulkan/VulkanFactory.h"

#include <SDL3/SDL_vulkan.h>

namespace Entix
{
    SDLFactory::SDLFactory()
    {
        if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
            Panic("Failed to initialize SDL!");

        if(!SDL_Vulkan_LoadLibrary(nullptr))
            Panic("Failed to load Vulkan library!");
    }

    SDLFactory::~SDLFactory()
    {
        SDL_Vulkan_UnloadLibrary();
        SDL_Quit();
    }

    Result<Window*> SDLFactory::CreateWindow(const WindowDesc& desc)
    {
        return new WindowSDL(desc);
    }

    Result<RHI::Factory*> SDLFactory::CreateRHIFactory(const RHI::FactoryDesc& desc)
    {
        EX_LET_TRY(requiredExtensions, GetRequiredVulkanInstanceExtensions());
        return new VulkanFactory(desc.applicationName, desc.applicationVersion, requiredExtensions);
    }

    void SDLFactory::PollEvents()
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

    Result<std::vector<const char*>> SDLFactory::GetRequiredVulkanInstanceExtensions()
    {
        Uint32 count;
        auto extensions = SDL_Vulkan_GetInstanceExtensions(&count);

        if(extensions == nullptr)
            return Error(SDL_GetError());

        return std::vector<const char*>(extensions, extensions + count);
    }
}