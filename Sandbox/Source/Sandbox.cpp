#include <Entix/Game/EntryPoint.h>
#include <Entix/Game/Application.h>

#include <Entix/RHI/Shader.h>

namespace Entix
{
    class SandboxApp : public Application
    {
    public:
        SandboxApp()
            : Application(ApplicationConfig())
        {
        }
    };

    Application* CreateApplication()
    {
        return new SandboxApp();
    }
}