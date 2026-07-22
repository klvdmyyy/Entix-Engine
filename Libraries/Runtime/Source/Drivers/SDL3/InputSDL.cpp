#include "Input/Base.h"

#include <SDL3/SDL.h>

void Input::GetCursorPosition(float& x, float& y)
{
    SDL_GetMouseState(&x, &y);
}

bool Input::IsButtonPressed(Uint8 button)
{
    return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_MASK(button);
}

bool Input::IsKeyModifierPressed(ModifierKey key)
{
    const bool* state = SDL_GetKeyboardState(nullptr);
    switch(key) {
        case ModifierKey::Ctrl:
            return state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL];
        case ModifierKey::Alt:
            return state[SDL_SCANCODE_LALT] || state[SDL_SCANCODE_RALT];
        case ModifierKey::Shift:
            return state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT];
        case ModifierKey::Super:
            return state[SDL_SCANCODE_LGUI] || state[SDL_SCANCODE_RGUI];
        default:
            return false;
    }
}

bool Input::IsKeyPressed(Uint8 scancode)
{
    const bool* state = SDL_GetKeyboardState(nullptr);
    return state[scancode];
}