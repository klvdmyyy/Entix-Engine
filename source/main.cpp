#include <Core/Application.h>

#include <Core/Debug/Log.h>
#include <Core/Debug/LogSinks.h>

#include <Core/EntryPoint.h>

#include <GUI/Window.h>

using namespace ERUNTIME_NAMESPACE;

static auto g_VBoxSpec = ApplicationSpecification {
    .name = "VBox Game",
    .description = "Voxel Sandbox Game",
    .windowSpec = {
        .width = 800,
        .height = 600,
        .title = "VBox 1.0",
    },
};

class ConsoleWindow : public GUI::Window {
public:
    ConsoleWindow()
        : GUI::Window("Console")
    {
        // Show window at startup
        Show();
    }
    
    void Draw() final
    {
    }
};

class VBoxApplication : public Application {
public:
    VBoxApplication()
        : Application(g_VBoxSpec)
    {
        Logger::Instance().AddSink(CreateScope<StdoutLogSink>());
        
        GetGUIContext().CreateWindow<ConsoleWindow>();
    }

    ~VBoxApplication()
    {
    }
};

Entix::Application* Entix::CreateApplication()
{
    return new VBoxApplication();
}