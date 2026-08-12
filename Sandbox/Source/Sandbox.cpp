#include <Entix/Game/EntryPoint.h>
#include <Entix/Game/Application.h>

#include <Entix/Core/IO/FileStream.h>
#include <Entix/Serialization/JsonArchive.h>

namespace Entix
{
    ApplicationDesc CreateApplication()
    {
        return {
            .threads = 2,
            .enableHotReload = true
        };
    }
}
