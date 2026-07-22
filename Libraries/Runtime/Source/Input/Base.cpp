#include "Input/Base.h"

#include <algorithm>

namespace Input {
    InputDevice ParseInputDevice(const String& name)
    {
        String device = name;
        std::transform(device.begin(), device.end(), device.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        if(device == "keyboard") return InputDevice::Keyboard;
        if(device == "mouse") return InputDevice::Mouse;

        return InputDevice::Unknown;
    }

    ModifierKey ParseModifierKey(const String& name)
    {
        String mod = name;
        std::transform(mod.begin(), mod.end(), mod.begin(), [](unsigned char c) {
            return std::tolower(c);
        });

        if(mod == "ctrl" || mod == "control")
            return ModifierKey::Ctrl;
        if(mod == "alt")
            return ModifierKey::Alt;
        if(mod == "shift")
            return ModifierKey::Shift;
        if(mod == "super" || mod == "gui" || mod == "win" || mod == "windows" || mod == "command" || mod == "cmd")
            return ModifierKey::Super;
        
        return ModifierKey::Unknown;
    }
}