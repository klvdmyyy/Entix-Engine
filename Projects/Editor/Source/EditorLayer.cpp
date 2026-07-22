#include "EditorLayer.h"

#include <Core/Debug/LogSinks.h>
#include <Core/IO/Decorators.h>

#include <Resources/ResourceManager.h>

#include <Input/Actions.h>
#include <Input/Events.h>

#include <GameFramework/TextureLoader.h>
#include <GameFramework/ObjMeshLoader.h>
#include <GameFramework/ShaderLoader.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

#include <tracy/Tracy.hpp>

EditorLayer::EditorLayer()
    : Layer("EditorLayer")
{
    EventBus::Instance().AddListener(this);
}

EditorLayer::~EditorLayer()
{
    EventBus::Instance().RemoveListener(this);
}

void EditorLayer::OnEvent(const Event& event)
{
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<MouseMotionEvent>([&](const MouseMotionEvent& event) {
        if(m_Editor_ViewportCameraRotation)
        {
            // TODO...
        }
        else if(m_Editor_ViewportGrabMouse)
        {
            m_editorCamera.yaw += event.k_xPosition * m_editorCameraSensitivity;
            m_editorCamera.pitch += event.k_yPosition * m_editorCameraSensitivity;
            
            m_editorCamera.pitch = std::clamp(m_editorCamera.pitch, -89.0f, 89.0f);
        }
    });
}

void EditorLayer::OnAttach()
{
    ZoneScoped;

    auto& rm = ResourceManager::Instance();

    // Setting editor Assets directory
    rm.SetAssetsDirectory(PROJECT_ASSETS_DIR);

    // Register resource loaders
    rm.RegisterLoader<ObjMeshLoader>(Application::Get().GetRendererContext());
    rm.RegisterLoader<ShaderLoader>(Application::Get().GetRendererContext());
    rm.RegisterLoader<TextureLoader>(Application::Get().GetRendererContext());

    ActionSystem::Instance().SetActionMap(ActionMap::LoadFromFile("C:\\Users\\User\\Desktop\\Entix-Engine\\Projects\\Editor\\action_map.json"));
    ActionSystem::Instance().PushContext({
        "Editor_ViewportCameraRotation",
        "Editor_ViewportGrabMouse",
        "Editor_ControllerMoveForward",
        "Editor_ControllerMoveBackward",
        "Editor_ControllerMoveLeft",
        "Editor_ControllerMoveRight",
    });

    // Viewport framebuffer
    Renderer::FramebufferSpecification viewportFramebufferSpec;
    viewportFramebufferSpec.attachmentSpec = {
        Renderer::FramebufferTextureFormat::RGBA8,
        Renderer::FramebufferTextureFormat::RED_INTEGER,
        Renderer::FramebufferTextureFormat::Depth
    };
    viewportFramebufferSpec.width = 800;
    viewportFramebufferSpec.height = 600;
    m_viewportFramebuffer = Ref<Renderer::Framebuffer>(Application::Get().GetRendererContext()->CreateFramebuffer(viewportFramebufferSpec));

    // Editor camera for viewport
    m_editorCamera.framebuffer = m_viewportFramebuffer;

    // Move camera
    m_editorCameraTransform.position.z = 5.0f;

    /////////////////////////////////////////////////////////////
    // Initialize testing scene

    auto& scene = Application::Get().GetCurrentScene();

    Entity cube = scene.CreateEntity("Cube");
    auto& cubeMesh = cube.AddComponent<StaticMeshComponent>();

    cubeMesh.vertexArray = rm.Load<Renderer::VertexArray, ObjMeshLoader>("Models/Cube.obj");
    cubeMesh.material.shader = rm.Load<Renderer::Shader, ShaderLoader>("Shaders/Textures.glsl");
    cubeMesh.material.texture = rm.Load<Renderer::Texture, TextureLoader>("Test.jpg");

    Entity child1 = scene.CreateEntity("Child 1");
    child1.AddOrReplaceComponent<TransformComponent>(cube);

    /////////////////////////////////////////////////////////////
    // Panels setup
    m_inspectorPanel.Setup();
    m_consolePanel.Setup();
    m_propertiesPanel.Setup();
}

void EditorLayer::OnTick(Timestep deltaTime)
{
    Application::Get().GetCurrentScene().OnTick(deltaTime);

    ////////////////////////////////////////
    ///////////// Framebuffer //////////////
    m_viewportFramebuffer->Resize(m_viewportSize.x, m_viewportSize.y);

    m_viewportFramebuffer->ClearAttachment(1, -1);

    //////////////////////////////
    ////////// Camera ////////////
    m_editorCameraTransform.UpdateLocalMatrix();
    m_editorCameraTransform.UpdateWorldMatrix(Float4x4(1.0f));

    Renderer::Rect fbRect = {
        m_viewportFramebuffer->GetSpecification().width,
        m_viewportFramebuffer->GetSpecification().height,
    };

    m_editorCamera.viewport.UpdateAbsolute(fbRect);
    m_editorCamera.Update(m_editorCameraTransform);

    /////////////////////////
    //////// Panels /////////
    m_inspectorPanel.Update();
    m_consolePanel.Update();
    m_propertiesPanel.Update();

    if(m_isViewportFocused)
    {
        ZoneScopedN("Viewport Controller");

        m_Editor_ViewportCameraRotation = Input::IsActionHeld("Editor_ViewportCameraRotation");
        m_Editor_ViewportGrabMouse = Input::IsActionHeld("Editor_ViewportGrabMouse");

        Application::Get().GetWindow()->GrabCursor(m_Editor_ViewportCameraRotation || m_Editor_ViewportGrabMouse);

        if(m_Editor_ViewportCameraRotation)
        {
            // TODO...
        }
        else if(m_Editor_ViewportGrabMouse)
        {
            if(Input::IsActionHeld("Editor_ControllerMoveForward"))
                m_editorCameraTransform.position -= (m_editorCamera.GetFront() * m_editorCameraSpeed) * (float)deltaTime;
            
            if(Input::IsActionHeld("Editor_ControllerMoveBackward"))
                m_editorCameraTransform.position += (m_editorCamera.GetFront() * m_editorCameraSpeed) * (float)deltaTime;
            
            if(Input::IsActionHeld("Editor_ControllerMoveLeft"))
                m_editorCameraTransform.position += (m_editorCamera.GetRight() * m_editorCameraSpeed) * (float)deltaTime;
            
            if(Input::IsActionHeld("Editor_ControllerMoveRight"))
                m_editorCameraTransform.position -= (m_editorCamera.GetRight() * m_editorCameraSpeed) * (float)deltaTime;
        }
        else
        {
            if(Int32 scroll = Input::GetMouseWheelScrollY(); scroll != 0)
            {
                m_editorCamera.fov -= static_cast<float>(scroll);
                m_editorCamera.fov = std::clamp(m_editorCamera.fov, 1.0f, 45.0f);
            }
        }
    }
}

void EditorLayer::OnRender()
{
    // Render the scene
    Application::Get().GetCurrentScene().OnRender(&m_editorCamera);

    {
        ZoneScopedN("ImGui - Docking Setup")

        // Docking setup
        ImGuiID dockspace_id = ImGui::GetID("Entix Editor Dockspace");
        ImGuiViewport* viewport = ImGui::GetMainViewport();

        if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr) {
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

            ImGuiID dock_id_left = 0;
            ImGuiID dock_id_right = 0;
            ImGuiID dock_id_bottom = 0;
            ImGuiID dock_id_main = dockspace_id;

            ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Down, 0.25f, &dock_id_bottom, &dock_id_main);
            ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Right, 0.20f, &dock_id_right, &dock_id_main);
            ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.20f, &dock_id_left, &dock_id_main);

            ImGui::DockBuilderDockWindow("Viewport", dock_id_main);

            ImGui::DockBuilderDockWindow("Inspector", dock_id_left);
            ImGui::DockBuilderDockWindow("Properties", dock_id_right);

            ImGui::DockBuilderDockWindow("Content Browser", dock_id_bottom);
            ImGui::DockBuilderDockWindow("Console", dock_id_bottom);

            ImGui::DockBuilderFinish(dockspace_id);
        }

        ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_PassthruCentralNode);
    }

    // Menu
    {
        ZoneScopedN("ImGui - Main Menu Bar");

        if(ImGui::BeginMainMenuBar()) {
            if(ImGui::BeginMenu("File")) {
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Edit")) {
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("View")) {
                if(ImGui::BeginMenu("Windows")) {
                    ImGui::MenuItem("Inspector", nullptr, &m_inspectorPanel.open);
                    ImGui::MenuItem("Properties", nullptr, &m_propertiesPanel.open);
                    ImGui::MenuItem("Viewport", nullptr, &m_viewportOpen);
                    ImGui::MenuItem("Content Browser", nullptr, &m_contentBrowserOpen);
                    ImGui::MenuItem("Console", nullptr, &m_consolePanel.open);
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    if(m_viewportOpen)
    {
        ZoneScopedN("ImGui - Viewport");

        ImGui::Begin("Viewport", &m_viewportOpen);

        m_isViewportFocused = ImGui::IsWindowFocused();

        m_viewportSize = ImGui::GetContentRegionAvail();

        ImGui::Image(reinterpret_cast<void*>(m_viewportFramebuffer->GetColorAttachmentRendererId()), m_viewportSize, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
    }

    m_inspectorPanel.Render();

    m_propertiesPanel.Render(m_inspectorPanel.GetSelectedEntity());

    if(m_contentBrowserOpen)
    {
        ZoneScopedN("ImGui - Content Browser");
        ImGui::Begin("Content Browser", &m_contentBrowserOpen);
        ImGui::End();
    }

    m_consolePanel.Render();
}