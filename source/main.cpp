#include <Core/Application.h>

#include <Core/Debug/Log.h>
#include <Core/Debug/LogSinks.h>

#include <Core/Resources/ResourceManager.h>

#include <Core/EntryPoint.h>

#include <WSI/ActionSystem.h>

#include <Renderer/Context.h>
#include <Renderer/VertexArray.h>
#include <Renderer/Buffer.h>

#include <GUI/ConsoleWindow.h>

#include <Scene/Entity.h>

#include "vbox.h"

static auto g_VBoxSpec = ApplicationSpecification {
    .name = "VBox Game",
    .description = "Voxel Sandbox Game",
    .windowSpec = {
        .width = 800,
        .height = 600,
        .title = "VBox 1.0",
    },
};

class VBoxApplication : public Application {
public:
    VBoxApplication()
        : Application(g_VBoxSpec),
          m_consoleWindow(GetGUIContext().CreateWindow<GUI::ConsoleWindow>())
    {
    }

    void OnInit() final
    {
        ResourceManager::Instance().LoadShader(VBOX_SIMPLE_SHADER);

        Entity camera = GetCurrentScene().CreateEntity("Camera");
        camera.AddComponent<CameraComponent>();

        TransformComponent& cameraTransform = camera.GetComponent<TransformComponent>();
        cameraTransform.position = Float3(0.0f, 0.0f, 2.0f);

        Entity square = GetCurrentScene().CreateEntity("Square");
        square.AddComponent<StaticMeshComponent>(CreateExampleMesh(GetRendererContext()));
    }

    void OnTick() final
    {
        if(ActionSystem::Instance().IsPressed("Console")) {
            if(m_consoleWindow.IsOpened())
                m_consoleWindow.Hide();
            else
                m_consoleWindow.Show();
        }
    }

private:
    GUI::ConsoleWindow& m_consoleWindow;
};

Entix::Application* Entix::CreateApplication()
{
    return new VBoxApplication();
}