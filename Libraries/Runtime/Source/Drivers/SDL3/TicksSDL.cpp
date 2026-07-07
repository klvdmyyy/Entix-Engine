#include "Core/Time.h"

#include <SDL3/SDL.h>

namespace Time {
    Uint32 GetTicks()
    {
        return SDL_GetTicks();
    }
}