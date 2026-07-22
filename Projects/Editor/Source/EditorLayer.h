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
#include <imgui_internal.h>

class EditorLayer : public Layer, EventListener {
public:
    EditorLayer();
    ~EditorLayer();

    void OnEvent(const Event& event) final;

    void OnAttach() final;

    void OnTick(Timestep deltaTime) final;
    void OnRender() final;

private:
    TransformComponent m_editorCameraTransform;
    CameraComponent m_editorCamera;

    float m_editorCameraSensitivity = 0.05f;
    float m_editorCameraSpeed = 5.0f;

    bool m_Editor_ViewportCameraRotation = false;
    bool m_Editor_ViewportGrabMouse = false;

    // Viewport
    bool m_viewportOpen = true;
    // bool m_isViewportFocused = false;
    bool m_isViewportHovered = false;
    bool m_setFocusOnViewport = false;
    ImVec2 m_viewportSize;
    Ref<Renderer::Framebuffer> m_viewportFramebuffer;
    std::array<ImRect, 4> m_guizmoToolsRect;
    Uint8 m_guizmoSelectedToolIndex = 0;
    Int8 m_guizmoHoveredToolIndex = -1;

    // Panels
    InspectorPanel m_inspectorPanel;
    ConsolePanel m_consolePanel;
    PropertiesPanel m_propertiesPanel;

    // Content Browser
    bool m_contentBrowserOpen = true;
};