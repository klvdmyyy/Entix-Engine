#pragma once

#include "PanelBase.h"

#include <Renderer/Texture.h>

#include <Resources/ResourceHandle.h>

#include <GameFramework.h>

class ConsolePanel : public EditorPanelBase {
public:
    ConsolePanel();

    void Setup();
    void Update() {}
    void Render();

    Int32 InputCallback(void* data_);

private:
    IO::Writer& m_writer;
    bool m_autoScroll = true;
    String m_inputBuffer;
    std::vector<String> m_history;
    Int32 m_historyIndex;

    ResourceHandle<Renderer::Texture> m_traceIcon;
    ResourceHandle<Renderer::Texture> m_infoIcon;
    ResourceHandle<Renderer::Texture> m_warnIcon;
    ResourceHandle<Renderer::Texture> m_errorIcon;
};