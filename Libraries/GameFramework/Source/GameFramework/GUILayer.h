// -*- mode: c++; -*-
#pragma once

#include "WSI/Window.h"

#include "Renderer/Context.h"

#include "GameFramework/Layer.h"

class GUILayer : public Layer {
public:
    GUILayer();

    void OnAttach() final;
    void OnDetach() final;

    void OnPreRender() final;
    void OnPostRender() final;

private:
    Renderer::Context* m_rendererContext{nullptr};
};