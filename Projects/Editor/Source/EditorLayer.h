#pragma once

#include "Panels/InspectorPanel.h"
#include "Panels/ConsolePanel.h"
#include "Panels/PropertiesPanel.h"

#include <GameFramework.h>

#include <Core/IO/Base.h>

#include <Renderer/Texture.h>
#include <Renderer/Framebuffer.h>

#include <Resources/ResourceHandle.h>

#include <imgui.h>

class EditorLayer : public Layer {
public:
    EditorLayer();

    void OnAttach() final;

    void OnTick(Timestep deltaTime) final;
    void OnRender() final;

private:
    TransformComponent m_editorCameraTransform;
    CameraComponent m_editorCamera;

    // Viewport
    bool m_viewportOpen = true;
    ImVec2 m_viewportSize;
    Ref<Renderer::Framebuffer> m_viewportFramebuffer;

    // Panels
    InspectorPanel m_inspectorPanel;
    ConsolePanel m_consolePanel;
    PropertiesPanel m_propertiesPanel;

    // Content Browser
    bool m_contentBrowserOpen = true;
};