#include <Entix/Game/EntryPoint.h>
#include <Entix/Game/Application.h>

#include <Entix/Core/IO/FileStream.h>
#include <Entix/Serialization/JsonArchive.h>

namespace Entix
{
    struct SerTest
    {
        int first = 10;
        String second = "Hello, World!";

        void Serialize(JsonArchive& ar)
        {
            ar & AField("first", first)
               & AField("second", second);
        }
    };

    class SandboxApp : public Application
    {
    public:
        SandboxApp()
            : Application(ApplicationConfig())
        {
            IO::FileStream stream("ArchiveTesting.json", IO::StreamMode::Write);
            JsonArchive ar(stream);

            int value = 10;

            std::vector<int> arrayValue = {1, 2, 3, 4, 5, 6, 7};
            std::unordered_map<String, int> seqValue = {
                {"a", 1},
                {"b", 2},
                {"c", 3}
            };

            SerTest structureValue;

            ar & AField("hello", value)
               & AField("world", value)
               & AField("array", arrayValue)
               & AField("sequence", seqValue)
               & AField("structure", structureValue);
        }
    };

    Application* CreateApplication()
    {
        return new SandboxApp();
    }
}
