#include "InspectorPanel.h"

#include <imgui.h>

#include <tracy/Tracy.hpp>

InspectorPanel::InspectorPanel()
    : EditorPanelBase("Inspector")
{
}

InspectorPanel::~InspectorPanel()
{
}

void InspectorPanel::Update()
{
    auto& scene = Application::Get().GetCurrentScene();
}

void InspectorPanel::Render()
{
    if(open)
    {
        ZoneScopedN("ImGui - Inspector");

        ImGui::Begin("Inspector", &open);

        auto& scene = Application::Get().GetCurrentScene();

        EntitySet entitySet;
        EntityChildMap childMap;

        scene.EachEntity([&](Entity entity) {
            auto parent = Entity{entity.GetComponent<TransformComponent>().parent, &scene};

            if(!childMap.contains(entity))
                childMap.insert({entity, {}});

            if(parent)
            {
                if(!childMap.contains(parent))
                    childMap.insert({parent, {}});
                
                childMap[parent].push_back(entity);
            }
        });

        for(auto& [entity, children] : childMap)
        {
            
            DrawEntityNode(entity, entitySet, childMap);
        }

        ImGui::End();
    }
}

void InspectorPanel::DrawEntityNode(Entity entity, EntitySet& entitySet, EntityChildMap& childMap)
{
    if(entitySet.contains(entity))
        return;
    else
        entitySet.insert(entity);

    auto tag = entity.GetComponent<TagComponent>().tag;
    ImGuiTreeNodeFlags flags = ((m_selectionContext.has_value() && m_selectionContext.value() == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

    auto& children = childMap[entity];

    if(children.empty())
        flags |= ImGuiTreeNodeFlags_Leaf;

    if(ImGui::TreeNodeEx((void*)(Uint64)(Uint32)entity, flags, tag.c_str())) {
        if(ImGui::IsItemClicked())
            m_selectionContext = entity;

        for(auto& child : children) {
            DrawEntityNode(child, entitySet, childMap);
        }

        ImGui::TreePop();
    }
    else
    {
        for(auto& child : children) {
            SkipEntityNode(child, entitySet, childMap);
        }
    }
}

void InspectorPanel::SkipEntityNode(Entity entity, EntitySet& entitySet, EntityChildMap& childMap)
{
    if(entitySet.contains(entity))
        return;
    else
        entitySet.insert(entity);

    for(auto& child : childMap[entity])
    {
        SkipEntityNode(child, entitySet, childMap);
    }
}