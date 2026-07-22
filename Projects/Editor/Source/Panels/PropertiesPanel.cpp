#include "PropertiesPanel.h"

#include <imgui.h>
#include <imgui_stdlib.h>

PropertiesPanel::PropertiesPanel()
    : EditorPanelBase("Properties")
{
}

void PropertiesPanel::Setup()
{
}

void PropertiesPanel::Update()
{
}

void PropertiesPanel::Render(std::optional<Entity> selectionContext)
{
    if(open)
    {
        ImGui::Begin("Properties", &open);
        if(selectionContext.has_value()) {
            auto selected = selectionContext.value();

            if(selected.HasComponent<IdComponent>())
            {
                if(ImGui::TreeNode("Id Component"))
                {
                    ImGui::Text("Uuid: %d", (Uint32)selected.GetComponent<IdComponent>().id);
                    ImGui::TreePop();
                }
            }

            if(selected.HasComponent<TagComponent>())
            {
                if(ImGui::TreeNode("Tag Component"))
                {
                    auto& tc = selected.GetComponent<TagComponent>();
                    String name = tc.tag;
                    if(ImGui::InputText("Name (Tag)", &name, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        tc.tag = name;
                    }
                    ImGui::TreePop();                    
                }
            }

            if(selected.HasComponent<TransformComponent>())
            {
                if(ImGui::TreeNode("Transform Component"))
                {
                    auto& transform = selected.GetComponent<TransformComponent>();
                    ImGui::InputFloat3("Position", &transform.position.x, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
                    ImGui::InputFloat3("Rotation", &transform.rotation.x, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
                    ImGui::InputFloat3("Scale", &transform.scale.x, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue);
                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
    }
}