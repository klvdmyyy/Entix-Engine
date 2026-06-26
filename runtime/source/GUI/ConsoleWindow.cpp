#include "GUI/ConsoleWindow.h"

#include "Core/Debug/LogSinks.h"
#include "Core/StringCommandRunner.h"

#include <algorithm>

#include <imgui.h>
#include <imgui_stdlib.h>

namespace ERUNTIME_NAMESPACE::GUI {
    ConsoleWindow::ConsoleWindow()
        : Window("Console")
    {
        // Hide console at startup.
        Hide();

        StringCommandRunner::Instance().AddCommand({ .name = "history", .description = "Prints all console history" },
                                                   [&](const CommandArgs& args) {
                                                       Debug::Info(LogCategory::Console, "HISTORY:");
                                                       for(const auto& entry : m_history)
                                                           Debug::Info(LogCategory::Console, "\t\t{}", entry);
                                                   });
    }

    void ConsoleWindow::DrawOutput()
    {
        if(m_autoScroll) {
            ImGui::SetScrollHereY(1.0f);
            m_autoScroll = false;
        }

        ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;

        ImGui::BeginChild("ConsoleOutput", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - 10), ImGuiChildFlags_None, flags);

        // Отображаем историю логов
        for (const auto& entry : BufferLogSink::Instance().GetEntries()) {
            ImGui::TextUnformatted(entry.c_str());
        }
        
        // Проверяем, прокрутил ли пользователь вверх
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            m_autoScroll = true;
        }
        
        ImGui::EndChild();
    }

    void ConsoleWindow::DrawInput()
    {
        ImGui::Separator();

        if(ImGui::InputText("##ConsoleInput", &m_inputBuffer,
                            ImGuiInputTextFlags_EnterReturnsTrue |
                            ImGuiInputTextFlags_CallbackCompletion |
                            ImGuiInputTextFlags_CallbackHistory,
                            [](ImGuiInputTextCallbackData* data) {
                                auto* self = static_cast<ConsoleWindow*>(data->UserData);
                                return self->InputCallback(data);
                            }, this)) {
            if(!m_inputBuffer.empty()) {
                m_history.push_back(m_inputBuffer);
                m_historyIndex = static_cast<int>(m_history.size());
                StringCommandRunner::Instance().Run(m_inputBuffer);
                m_inputBuffer = "";
                m_autoScroll = true;
            }
            ImGui::SetKeyboardFocusHere(-1);
        }
    }

    int ConsoleWindow::InputCallback(void* data_)
    {
        ImGuiInputTextCallbackData* data = static_cast<ImGuiInputTextCallbackData*>(data_);
        if(data->EventFlag == ImGuiInputTextFlags_CallbackCompletion) {
            Debug::Error(LogCategory::Console, "Console doesn't support completion for now!");
            return 1;
        }

        if(data->EventFlag == ImGuiInputTextFlags_CallbackHistory) {
            if (data->EventKey == ImGuiKey_UpArrow) {
                m_historyIndex = std::max(m_historyIndex - 1, 0);
            } else if (data->EventKey == ImGuiKey_DownArrow) {
                m_historyIndex = std::min(m_historyIndex + 1, (int)m_history.size());
            }
            
            if (m_historyIndex >= 0 && m_historyIndex < (int)m_history.size()) {
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, m_history[m_historyIndex].c_str());
            } else if (m_historyIndex == m_history.size()) {
                data->DeleteChars(0, data->BufTextLen);
            }
            return 1;
        }

        // if (data->EventFlag == ImGuiInputTextFlags_EnterReturnsTrue) {
        //     String cmd(data->Buf);
        //     if(!cmd.empty()) {
        //         m_history.push_back(cmd);
        //         m_historyIndex = (int)m_history.size();
        //         StringCommandRunner::Instance().Run(cmd);
        //         data->Buf[0] = '\0';
        //         m_autoScroll = true;
        //     }
        // }
        
        return 0;
    }
}