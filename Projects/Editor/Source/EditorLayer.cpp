#include "EditorLayer.h"

#include <Core/Debug/LogSinks.h>
#include <Core/IO/Decorators.h>

#include <Resources/ResourceManager.h>

#include <Input/Actions.h>

#include <GameFramework/TextureLoader.h>

#include <imgui.h>
#include <imgui_stdlib.h>

EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_consoleWriter(BufferLogSink::Instance())
{
}

void EditorLayer::OnAttach()
{
    auto& rm = ResourceManager::Instance();

    rm.SetAssetsDirectory(PROJECT_ASSETS_DIR);

    ActionSystem::Instance().SetActionMap(ActionMap::LoadFromFile("C:\\Users\\User\\Desktop\\Entix-Engine\\Projects\\Editor\\action_map.json"));
    ActionSystem::Instance().PushContext({"Console"});

    rm.RegisterLoader<TextureLoader>(Application::Get().GetRendererContext());

    rm.Load<Renderer::Texture, TextureLoader>("Test.jpg");

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
}

void EditorLayer::OnTick([[maybe_unused]] Timestep deltaTime)
{
    Application::Get().GetCurrentScene().OnTick(deltaTime);

    if(Input::IsActionPressed("Console"))
        m_consoleOpen = !m_consoleOpen;
}

void EditorLayer::OnRender()
{
    // Render the scene
    Application::Get().GetCurrentScene().OnRender();

    // Developer console
    if(m_consoleOpen)
    {
        ImGui::Begin("Console", &m_consoleOpen);

        // Console Output
        {
            ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
            ImGui::BeginChild("ConsoleOutput", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - 10), ImGuiChildFlags_None, flags);

            for(const auto& entry : BufferLogSink::Instance().GetEntries()) {
                ImGui::TextUnformatted(entry.c_str());
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

                    writer.WriteFmt("> {}", m_consoleInputBuffer);
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
        auto suggestions = StringCommandRunner::Instance().GetSuggestions(data->Buf);

        if(suggestions.size() == 1) {
            data->DeleteChars(0, data->BufTextLen);
            data->InsertChars(0, (suggestions[0] + " ").c_str());
        } else if(suggestions.size() > 1) {
            auto writer = IO::TextWriter::CreateNonOwned(m_consoleWriter);
            writer.Write("Options: ");
            for(const auto& s : suggestions) {
                writer.WriteFmt("\t{}", s);
            }
        }

        return 1;
    }

    if(data->EventFlag == ImGuiInputTextFlags_CallbackHistory)
    {
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