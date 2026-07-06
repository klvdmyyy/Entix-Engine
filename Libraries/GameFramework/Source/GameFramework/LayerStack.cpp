#include "GameFramework/LayerStack.h"

Layer* LayerStack::PushLayer(Ref<Layer> layer)
{
    m_layers.emplace(m_layers.begin() + m_layerInsertIndex, std::move(layer));
    Layer* result = m_layers[m_layerInsertIndex++].get();
    result->OnAttach();
    return result;
}

Layer* LayerStack::PushOverlay(Ref<Layer> overlay)
{
    m_layers.emplace_back(std::move(overlay));
    Layer* result = m_layers.back().get();
    result->OnAttach();
    return result;
}

void LayerStack::PopLayer(const Ref<Layer>& layer)
{
    auto it = std::find(m_layers.begin(), m_layers.begin() + m_layerInsertIndex, layer);
    if(it != m_layers.begin() + m_layerInsertIndex) {
        layer->OnDetach();
        m_layers.erase(it);
        m_layerInsertIndex--;
    }
}

void LayerStack::PopOverlay(const Ref<Layer>& overlay)
{
    auto it = std::find(m_layers.begin() + m_layerInsertIndex, m_layers.end(), overlay);
    if(it != m_layers.end()) {
        overlay->OnDetach();
        m_layers.erase(it);
    }
}