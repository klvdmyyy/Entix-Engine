#include <Entix/Game/EntryPoint.h>
#include <Entix/Game/Application.h>

namespace Entix
{
    class SandboxApp : public Application
    {
    };

    Application* CreateApplication()
    {
        return new SandboxApp();
    }
}