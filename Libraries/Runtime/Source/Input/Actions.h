#pragma once

#include "Core/Base.h"
#include "Core/String.h"

#include "Input/ActionSystem.h"

namespace Input {
    FORCE_INLINE
    inline bool IsActionPressed(const String& actionName)
    {
        return ::ActionSystem::Instance().IsPressed(actionName);
    }

    FORCE_INLINE
    inline bool IsActionHeld(const String& actionName)
    {
        return ::ActionSystem::Instance().IsHeld(actionName);
    }

    FORCE_INLINE
    inline bool IsActionReleased(const String& actionName)
    {
        return ::ActionSystem::Instance().IsReleased(actionName);
    }
}