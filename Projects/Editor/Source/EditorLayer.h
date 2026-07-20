#pragma once

#include <GameFramework.h>

#include <Core/IO/Base.h>

#include <Renderer/Texture.h>

#include <Resources/ResourceHandle.h>

class EditorLayer : public Layer {
public:
    EditorLayer();

    void OnAttach() final;

    void OnTick(Timestep deltaTime) final;
    void OnRender() final;

    Int32 InputCallback(void* data_);

private:
    // Developer Console
    bool m_consoleOpen = false;
    IO::Writer& m_consoleWriter;
    bool m_consoleAutoScroll = true;
    String m_consoleInputBuffer;
    std::vector<String> m_consoleHistory;
    Int32 m_consoleHistoryIndex;

    // Logging thumbnails (for console)
    ResourceHandle<Renderer::Texture> m_traceIcon;
    ResourceHandle<Renderer::Texture> m_infoIcon;
    ResourceHandle<Renderer::Texture> m_warnIcon;
    ResourceHandle<Renderer::Texture> m_errorIcon;
};