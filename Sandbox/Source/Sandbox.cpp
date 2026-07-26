#include <Entix/Game/EntryPoint.h>
#include <Entix/Game/Application.h>

#include <Entix/Core/Panic.h>

namespace Entix
{
    class SandboxApp : public Application
    {
    public:
        SandboxApp()
        {
            Panic("Hello, World!");
        }
    };

    Application* CreateApplication()
    {
        return new SandboxApp();
    }
}