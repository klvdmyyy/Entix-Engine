#include "Entix/Core/Events/Dispatcher.h"

namespace Entix
{
    EventBus& EventBus::Instance()
    {
        static EventBus s_instance;
        return s_instance;
    }
}