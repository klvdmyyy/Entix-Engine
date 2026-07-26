#include <Entix/Game/EntryPoint.h>
#include <Entix/Game/Application.h>

#include <Entix/Core/Panic.h>

#include <Entix/Core/Events/Dispatcher.h>

namespace Entix
{
    DEFINE_EVENT_CATEGORY(MyEventCategory);

    struct MyEvent : public Event<MyEvent> {
        using Category = MyEventCategory;

        String message;
    };

    class SandboxApp : public Application
    {
    public:
        SandboxApp()
        {
            EventBus::Instance().Subscribe<MyEvent>([](const MyEvent& event){
                EX_LOG(LogTemp, Warning, "{}", event.message);
            });

            EventBus::Instance().Send(MyEvent{.message = "Hello, World!"});
        }
    };

    Application* CreateApplication()
    {
        return new SandboxApp();
    }
}