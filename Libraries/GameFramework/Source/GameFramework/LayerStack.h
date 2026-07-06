// -*- mode: c++; -*-
#pragma once

#include "Core/Memory.h"

#include "GameFramework/Layer.h"

#include <vector>
#include <algorithm>

class LayerStack {
public:
    LayerStack() = default;
    ~LayerStack() = default;

    Layer* PushLayer(Ref<Layer> layer);
    Layer* PushOverlay(Ref<Layer> overlay);
    
    void PopLayer(const Ref<Layer>& layer);
    void PopOverlay(const Ref<Layer>& overlay);

    std::vector<Ref<Layer>>::iterator begin() { return m_layers.begin(); }
    std::vector<Ref<Layer>>::iterator end() { return m_layers.end(); }
    std::vector<Ref<Layer>>::reverse_iterator rbegin() { return m_layers.rbegin(); }
    std::vector<Ref<Layer>>::reverse_iterator rend() { return m_layers.rend(); }

    std::vector<Ref<Layer>>::const_iterator begin() const { return m_layers.begin(); }
    std::vector<Ref<Layer>>::const_iterator end() const { return m_layers.end(); }
    std::vector<Ref<Layer>>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
    std::vector<Ref<Layer>>::const_reverse_iterator rend() const { return m_layers.rend(); }

private:
    std::vector<Ref<Layer>> m_layers;
    std::size_t m_layerInsertIndex{0};
};