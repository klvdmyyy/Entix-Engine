#pragma once

#include "Entix/Game/ControlFlow.h"
#include "Entix/Game/WorldContext.h"

#include "Entix/Game/Layer.h"

namespace Entix
{
    class LayerStack
    {
    public:
        LayerStack(ControlFlow* controlFlow, WorldContext* worldContext);

        void PushLayer(Scope<Layer> layer);
        void PushOverlay(Scope<Layer> overlay);

        std::vector<Scope<Layer>>::iterator begin() noexcept { return m_layers.begin(); }
        std::vector<Scope<Layer>>::iterator end() noexcept { return m_layers.end(); }
        std::vector<Scope<Layer>>::const_iterator begin() const noexcept { return m_layers.begin(); }
        std::vector<Scope<Layer>>::const_iterator end() const noexcept { return m_layers.end(); }

        std::vector<Scope<Layer>>::reverse_iterator rbegin() noexcept { return m_layers.rbegin(); }
        std::vector<Scope<Layer>>::reverse_iterator rend() noexcept { return m_layers.rend(); }
        std::vector<Scope<Layer>>::const_reverse_iterator rbegin() const noexcept { return m_layers.rbegin(); }
        std::vector<Scope<Layer>>::const_reverse_iterator rend() const noexcept { return m_layers.rend(); }

    private:
        ControlFlow* m_controlFlow;
        WorldContext* m_worldContext;

        std::vector<Scope<Layer>> m_layers{};
    };
}
