#include <Core/Application.h>

#include <Core/Debug/Log.h>
#include <Core/Debug/LogSinks.h>

#include <Core/EntryPoint.h>

using namespace ERUNTIME_NAMESPACE;

static auto g_VBoxSpec = ApplicationSpecification {
    .Name = "VBox Game",
    .Description = "Voxel Sandbox Game",
    .WindowSpec = {
        .Width = 800,
        .Height = 600,
        .Title = "VBox 1.0",
    },
};

class VBoxApplication : public Application {
public:
    VBoxApplication()
        : Application(g_VBoxSpec)
    {
        Logger::Instance().AddSink(CreateScope<StdoutLogSink>());

        Debug::Error(LogCategory::IO, "Failed to read file!");
        EX_LOG(Error, LogCategory::IO, "Failed to read file!");
    }

    ~VBoxApplication()
    {
    }
};

Entix::Application* Entix::CreateApplication()
{
    return new VBoxApplication();
}