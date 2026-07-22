// -*- mode: c++; -*-
#pragma once

#include "Core/Window.h"

#include "Renderer/Context.h"

#include "GameFramework/Layer.h"

#include "Core/Debug/LogEntry.h"

class GUILayer : public Layer {
public:
    static const LogCategory sk_logCategory;

    GUILayer();

    void OnAttach() final;
    void OnDetach() final;

    void OnPreRender() final;
    void OnPostRender() final;

    void* GetImGuiContext() const noexcept;

private:
    Renderer::Context* m_rendererContext{nullptr};
};