#include <Entix/Game/EntryPoint.h>
#include <Entix/Game/Application.h>

#include <Entix/Core/IO/FileStream.h>
#include <Entix/Serialization/JsonArchive.h>

namespace Entix
{
    ApplicationDesc CreateApplication(int argc, char** argv)
    {
        return ApplicationDescBuilder()
            .SetCLIArgs(argc, argv)
            .SetName("Sandbox")
            .SetVersion({0, 1, 0})
            .SetThreadCount(2)
            .EnableResourceHotReload()
            .Build()
            .Unwrap();
    }
}
