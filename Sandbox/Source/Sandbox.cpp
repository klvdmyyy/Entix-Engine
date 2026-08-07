#include <Entix/Game/EntryPoint.h>
#include <Entix/Game/Application.h>

#include <Entix/Core/IO/FileStream.h>
#include <Entix/Serialization/JsonArchive.h>

namespace Entix
{
    class SandboxApp : public Application
    {
    public:
        SandboxApp()
        {
        }
    };

    Application* CreateApplication()
    {
        return new SandboxApp();
    }
}
