#include <Entix/Game/EntryPoint.h>
#include <Entix/Game/Application.h>

#include <Entix/Core/IO/FileStream.h>
#include <Entix/Serialization/JsonArchive.h>

namespace Entix
{
    struct SerTest
    {
        Uint32 first;
        Int32 second;
        String third;

        void Serialize(JsonArchive& ar)
        {
            ar & AField("first", first)
               & AField("second", second)
               & AField("third", third);
        }

        static SerTest Deserialize(JsonArchive& ar)
        {
            SerTest res;

            ar & AField("first", res.first)
               & AField("second", res.second)
               & AField("third", res.third);

            return res;
        }
    };

    class SandboxApp : public Application
    {
    public:
        SandboxApp()
        {
            IO::FileStream stream("ArchiveTesting.json", IO::StreamMode::Read);
            JsonArchive ar(stream);

            auto serTest = SerTest::Deserialize(ar);

            EX_LOG(LogTemp, Info, "first: {} second: {} third: {}", serTest.first, serTest.second, serTest.third);
        }
    };

    Application* CreateApplication()
    {
        return new SandboxApp();
    }
}
