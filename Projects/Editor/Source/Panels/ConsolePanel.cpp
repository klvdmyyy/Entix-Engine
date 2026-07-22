#include "ConsolePanel.h"

#include <Core/Debug/LogSinks.h>
#include <Core/IO/Decorators.h>

#include <Resources/ResourceManager.h>

#include <GameFramework/TextureLoader.h>

#include <imgui.h>
#include <imgui_stdlib.h>

#include <tracy/Tracy.hpp>

ConsolePanel::ConsolePanel()
    : EditorPanelBase("Console"), m_writer(BufferLogSink::Instance())
{
}

void ConsolePanel::Setup()
{
    auto& rm = ResourceManager::Instance();

    // Loading thumbnails
    m_traceIcon = rm.Load<Renderer::Texture, TextureLoader>("Thumbnails/TraceLog96x96.png");
    m_infoIcon = rm.Load<Renderer::Texture, TextureLoader>("Thumbnails/InfoLog96x96.png");
    m_warnIcon = rm.Load<Renderer::Texture, TextureLoader>("Thumbnails/WarningLog96x96.png");
    m_errorIcon = rm.Load<Renderer::Texture, TextureLoader>("Thumbnails/ErrorLog96x96.png");
}

void ConsolePanel::Render()
{
    ZoneScopedN("ImGui - Console");

    if(open) {
        ImGui::Begin("Console", &open);
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
                m_autoScroll = true;
            }
            
            if(m_autoScroll) {
                ImGui::SetScrollHereY(1.0f);
                m_autoScroll = false;
            }
            
            ImGui::EndChild();
        }

        // Console Input
        {
            ZoneScopedN("ImGui - Console Input");

            ImGui::Separator();

            if(ImGui::InputText("##ConsoleInput", &m_inputBuffer,
                            ImGuiInputTextFlags_EnterReturnsTrue |
                            ImGuiInputTextFlags_CallbackCompletion |
                            ImGuiInputTextFlags_CallbackHistory,
                            [](ImGuiInputTextCallbackData* data) {
                                auto* self = static_cast<ConsolePanel*>(data->UserData);
                                return self->InputCallback(data);
                            }, this)) {
                if(!m_inputBuffer.empty()) {
                    m_history.push_back(m_inputBuffer);
                    m_historyIndex = static_cast<Int32>(m_history.size());

                    auto writer = IO::TextWriter::CreateNonOwned(m_writer);

                    // writer.WriteFmt("> {}", m_inputBuffer);
                    StringCommandRunner::Instance().Run(m_inputBuffer, writer);

                    m_inputBuffer = "";
                    m_autoScroll = true;
                }
                ImGui::SetKeyboardFocusHere(-1);
            }
        }
        ImGui::End();
    }
}

Int32 ConsolePanel::InputCallback(void* data_)
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
            auto bufWriter = IO::BufferedWriter::CreateNonOwned(m_writer);
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
            m_historyIndex = std::max(m_historyIndex - 1, 0);
        } else if(data->EventKey == ImGuiKey_DownArrow) {
            m_historyIndex = std::min(m_historyIndex + 1, static_cast<Int32>(m_history.size()));
        }

        if (m_historyIndex >= 0 && m_historyIndex < (int)m_history.size()) {
            data->DeleteChars(0, data->BufTextLen);
            data->InsertChars(0, m_history[m_historyIndex].c_str());
        } else if (m_historyIndex == m_history.size()) {
            data->DeleteChars(0, data->BufTextLen);
        }
        return 1;
    }

    return 0;
}