#pragma once

#include "PanelBase.h"

#include <GameFramework.h>

#include <optional>
#include <unordered_set>
#include <map>
#include <vector>

class InspectorPanel : public EditorPanelBase
{
public:
    using EntitySet = std::unordered_set<Entity>;
    using EntityChildMap = std::map<Entity, std::vector<Entity>>;

    InspectorPanel();
    ~InspectorPanel();

    void Setup() {}
    void Update();
    void Render();

    std::optional<Entity> GetSelectedEntity() const noexcept
    {
        return m_selectionContext;
    }

private:
    void DrawEntityNode(Entity entity, EntitySet& entitySet, EntityChildMap& childMap);
    void SkipEntityNode(Entity entity, EntitySet& entitySet, EntityChildMap& childMap);
    
    std::optional<Entity> m_selectionContext = std::nullopt;
};