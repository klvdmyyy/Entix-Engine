#include "Entix/Game/LayerStack.h"

namespace Entix
{
    LayerStack::LayerStack(ControlFlow* controlFlow, WorldContext* worldContext)
        : m_controlFlow(controlFlow),
          m_worldContext(worldContext)
    {
    }

    void LayerStack::PushLayer(Scope<Layer> layer)
    {
        layer->m_controlFlow = m_controlFlow;
        layer->m_worldContext = m_worldContext;
    }

    void LayerStack::PushOverlay(Scope<Layer> overlay)
    {
        overlay->m_controlFlow = m_controlFlow;
        overlay->m_worldContext = m_worldContext;
    }
}
