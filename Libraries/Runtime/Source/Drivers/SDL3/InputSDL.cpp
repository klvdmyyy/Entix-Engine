#include "WSI/Input.h"

#include <SDL3/SDL.h>

void Input::GetCursorPosition(float& x, float& y)
{
    SDL_GetMouseState(&x, &y);
}

bool Input::IsKeyPressed(uint8_t scancode)
{
    const bool* state = SDL_GetKeyboardState(nullptr);
    return state[scancode];
}