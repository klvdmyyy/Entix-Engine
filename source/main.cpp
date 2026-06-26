#include <Core/Application.h>

#include <Core/Debug/Log.h>
#include <Core/Debug/LogSinks.h>

#include <Core/EntryPoint.h>

#include <WSI/ActionSystem.h>

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
        : Application(g_VBoxSpec),
          m_consoleWindow(GetGUIContext().CreateWindow<ConsoleWindow>())
    {
        Logger::Instance().AddSink(CreateScope<StdoutLogSink>());
    }

    void OnInit() final
    {
        ActionSystem::Instance().PushContext(ActionContext{"Console"});
        ActionSystem::Instance().SetActionMap(ActionMap::LoadFromFile(VBOX_DEFAULT_ACTION_MAP));
    }

    void OnUpdate() final
    {
        if(ActionSystem::Instance().IsPressed("Console")) {
            if(m_consoleWindow.IsOpened())
                m_consoleWindow.Hide();
            else
                m_consoleWindow.Show();
        }
    }

private:
    ConsoleWindow& m_consoleWindow;
};

Entix::Application* Entix::CreateApplication()
{
    return new VBoxApplication();
}