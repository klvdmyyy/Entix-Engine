#include "PanelBase.h"

#include <GameFramework.h>

#include <optional>

class PropertiesPanel : public EditorPanelBase {
public:
    PropertiesPanel();

    void Setup();
    void Update();
    void Render(std::optional<Entity> selectionContext);
};