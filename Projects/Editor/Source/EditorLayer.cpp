#include "EditorLayer.h"

#include <Core/Debug/LogSinks.h>
#include <Core/IO/Decorators.h>

#include <Resources/ResourceManager.h>

#include <Input/Actions.h>

#include <GameFramework/TextureLoader.h>
#include <GameFramework/ObjMeshLoader.h>
#include <GameFramework/ShaderLoader.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>

#include <tracy/Tracy.hpp>

EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_consoleWriter(BufferLogSink::Instance())
{
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
    ActionSystem::Instance().PushContext({"Console"});

    // Loading thumbnails
    m_traceIcon = rm.Load<Renderer::Texture, TextureLoader>("Thumbnails/TraceLog96x96.png");
    m_infoIcon = rm.Load<Renderer::Texture, TextureLoader>("Thumbnails/InfoLog96x96.png");
    m_warnIcon = rm.Load<Renderer::Texture, TextureLoader>("Thumbnails/WarningLog96x96.png");
    m_errorIcon = rm.Load<Renderer::Texture, TextureLoader>("Thumbnails/ErrorLog96x96.png");

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

    // Editor commands
    StringCommandRunner::Instance().AddCommand({ .name = "e_show_console", .description = "0 - Hide, 1 - Show" },
    [&](const CommandArgs& args, IO::Writer& rawWriter) {
        auto writer = IO::TextWriter::CreateNonOwned(rawWriter);

        if(args.Count() != 1) {
            writer.Write("Usage: e_show_console [0/1]");
            return;
        }

        if(args.Get(0) == "0") {
            m_consoleOpen = false;
        } else {
            m_consoleOpen = true;
        }
    });

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
}

void EditorLayer::OnTick([[maybe_unused]] Timestep deltaTime)
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

    /////////////////////////
    //////// Other //////////
    if(Input::IsActionPressed("Console"))
        m_consoleOpen = !m_consoleOpen;
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
                    ImGui::MenuItem("Properties", nullptr, &m_propertiesOpen);
                    ImGui::MenuItem("Viewport", nullptr, &m_viewportOpen);
                    ImGui::MenuItem("Content Browser", nullptr, &m_contentBrowserOpen);
                    ImGui::MenuItem("Console", "`", &m_consoleOpen);
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

        m_viewportSize = ImGui::GetContentRegionAvail();

        ImGui::Image(reinterpret_cast<void*>(m_viewportFramebuffer->GetColorAttachmentRendererId()), m_viewportSize, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
    }

    m_inspectorPanel.Render();

    if(m_propertiesOpen)
    {
        ZoneScopedN("ImGui - Properties");
        ImGui::Begin("Properties", &m_propertiesOpen);
        ImGui::End();
    }

    if(m_contentBrowserOpen)
    {
        ZoneScopedN("ImGui - Content Browser");
        ImGui::Begin("Content Browser", &m_contentBrowserOpen);
        ImGui::End();
    }

    // Developer console
    if(m_consoleOpen)
    {
        ZoneScopedN("ImGui - Console");

        ImGui::Begin("Console", &m_consoleOpen);

        // Console Output
        {
            ZoneScopedN("ImGui - Console Output");

            ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
            ImGui::BeginChild("ConsoleOutput", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - 10), ImGuiChildFlags_None, flags);

            const float windowWidth = ImGui::GetContentRegionAvail().x;
            const float iconSize = 20.0f;
            const float padding = 4.0f;

            Uint32 index = 0;
            for(const BufferLogSink::Entry& entry : BufferLogSink::Instance().GetEntries()) {
                float textWidth = windowWidth - iconSize - padding * 3.0f;
                if(textWidth < 50.0f) textWidth = 50.0f;

                ImVec2 textSize = ImGui::CalcTextSize(entry.message.c_str(), nullptr, false, textWidth);
                float cellHeight = textSize.y + padding * 2;

                ImVec2 cursor = ImGui::GetCursorScreenPos();
                ImDrawList* drawList = ImGui::GetWindowDrawList();

                ImU32 bgColor = ImGui::GetColorU32(ImGuiCol_FrameBg);

                drawList->AddRectFilled(cursor, ImVec2(cursor.x + windowWidth, cursor.y + cellHeight), bgColor);

                drawList->AddRect(cursor, ImVec2(cursor.x + windowWidth, cursor.y + cellHeight), ImGui::GetColorU32(ImGuiCol_Border));

                if(entry.level.has_value()) {
                    ImVec2 iconPos(cursor.x + padding, cursor.y + (cellHeight - iconSize) * 0.5f);
                    ImTextureID icon;

                    switch(entry.level.value()) {
                        case LogLevel::Trace: icon = m_traceIcon->GetRendererId(); break;
                        case LogLevel::Info: icon = m_infoIcon->GetRendererId(); break;
                        case LogLevel::Warning: icon = m_warnIcon->GetRendererId(); break;
                        case LogLevel::Error: icon = m_errorIcon->GetRendererId(); break;
                        case LogLevel::Critical: icon = m_errorIcon->GetRendererId(); break;
                    }
                    drawList->AddImage(static_cast<intptr_t>(icon), iconPos, ImVec2(iconPos.x + iconSize, iconPos.y + iconSize), ImVec2(0, 1), ImVec2(1, 0));
                }

                ImVec2 textPos(cursor.x + padding + iconSize + padding, cursor.y + padding);
                ImGui::SetCursorScreenPos(textPos);
                ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + textWidth);
                ImGui::TextUnformatted(entry.message.c_str());
                ImGui::PopTextWrapPos();

                ImGui::SetCursorScreenPos(ImVec2(cursor.x, cursor.y + cellHeight + 2.0f));

                ImGui::Dummy(ImVec2(1.0f, 1.0f));
            }

            // Проверяем, прокрутил ли пользователь вверх
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                m_consoleAutoScroll = true;
            }
            
            if(m_consoleAutoScroll) {
                ImGui::SetScrollHereY(1.0f);
                m_consoleAutoScroll = false;
            }
            
            ImGui::EndChild();
        }

        // Console Input
        {
            ZoneScopedN("ImGui - Console Input");

            ImGui::Separator();

            if(ImGui::InputText("##ConsoleInput", &m_consoleInputBuffer,
                            ImGuiInputTextFlags_EnterReturnsTrue |
                            ImGuiInputTextFlags_CallbackCompletion |
                            ImGuiInputTextFlags_CallbackHistory,
                            [](ImGuiInputTextCallbackData* data) {
                                auto* self = static_cast<EditorLayer*>(data->UserData);
                                return self->InputCallback(data);
                            }, this)) {
                if(!m_consoleInputBuffer.empty()) {
                    m_consoleHistory.push_back(m_consoleInputBuffer);
                    m_consoleHistoryIndex = static_cast<Int32>(m_consoleHistory.size());

                    auto writer = IO::TextWriter::CreateNonOwned(m_consoleWriter);

                    // writer.WriteFmt("> {}", m_consoleInputBuffer);
                    StringCommandRunner::Instance().Run(m_consoleInputBuffer, writer);

                    m_consoleInputBuffer = "";
                    m_consoleAutoScroll = true;
                }
                ImGui::SetKeyboardFocusHere(-1);
            }
        }
        ImGui::End();
    }
}

Int32 EditorLayer::InputCallback(void* data_)
{
    ImGuiInputTextCallbackData* data = static_cast<ImGuiInputTextCallbackData*>(data_);

    if(data->EventFlag == ImGuiInputTextFlags_CallbackCompletion)
    {
        ZoneScopedN("ImGui - Console Completion Callback");

        auto suggestions = StringCommandRunner::Instance().GetSuggestions(data->Buf);

        if(suggestions.size() == 1) {
            data->DeleteChars(0, data->BufTextLen);
            data->InsertChars(0, (suggestions[0] + " ").c_str());
        } else if(suggestions.size() > 1) {
            auto bufWriter = IO::BufferedWriter::CreateNonOwned(m_consoleWriter);
            auto writer = IO::TextWriter::CreateNonOwned(bufWriter);
            writer.Write("Options: ");
            for(const auto& s : suggestions) {
                writer.WriteFmt("\n\t{}", s);
            }
        }

        return 1;
    }

    if(data->EventFlag == ImGuiInputTextFlags_CallbackHistory)
    {
        ZoneScopedN("ImGui - Console History Callback");

        if(data->EventKey == ImGuiKey_UpArrow) {
            m_consoleHistoryIndex = std::max(m_consoleHistoryIndex - 1, 0);
        } else if(data->EventKey == ImGuiKey_DownArrow) {
            m_consoleHistoryIndex = std::min(m_consoleHistoryIndex + 1, static_cast<Int32>(m_consoleHistory.size()));
        }

        if (m_consoleHistoryIndex >= 0 && m_consoleHistoryIndex < (int)m_consoleHistory.size()) {
            data->DeleteChars(0, data->BufTextLen);
            data->InsertChars(0, m_consoleHistory[m_consoleHistoryIndex].c_str());
        } else if (m_consoleHistoryIndex == m_consoleHistory.size()) {
            data->DeleteChars(0, data->BufTextLen);
        }
        return 1;
    }

    return 0;
}