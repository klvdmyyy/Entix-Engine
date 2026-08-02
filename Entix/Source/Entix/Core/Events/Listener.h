#pragma once

#include "Entix/Core/Events/Base.h"

namespace Entix
{
    class EventListener
    {
    public:
        virtual void OnEvent(const Event& e) = 0;
    };
}