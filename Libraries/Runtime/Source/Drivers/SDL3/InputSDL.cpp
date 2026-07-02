#include "WSI/Input.h"

#include <SDL3/SDL.h>

bool Input::IsKeyPressed(uint8_t scancode)
{
    const bool* state = SDL_GetKeyboardState(nullptr);
    return state[scancode];
}