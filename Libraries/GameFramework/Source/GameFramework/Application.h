// -*- mode: c++; -*-
#pragma once

#include "Core/String.h"
#include "Core/Memory.h"
#include "Core/EventSystem.h"

#include "Core/Debug/Log.h"
#include "Core/StringCommandRunner.h"

#include "WSI/Window.h"

#include "Renderer/Context.h"

#include "GameFramework/Scene.h"
#include "GameFramework/LayerStack.h"

#include <type_traits>

struct ApplicationSpecification
{
    String name;
    String description;
    WindowSpecification windowSpec;
};

class Application : EventListener
{
 public:
    ~Application();

    static Application& Get() { return *s_instance; }

    void Run(int argc, char** argv);

    void OnEvent(const Event& event) final;

    ApplicationSpecification GetSpec() const noexcept { return k_spec; }

    Window* GetWindow() const { return m_window.get(); }
    Renderer::Context* GetRendererContext() const { return m_rendererContext.get(); }
    Scene& GetCurrentScene() const { return *m_scene; }

    template<std::derived_from<Layer> T, typename... Args>
    FORCE_INLINE
    inline constexpr Layer* PushLayer(Args&&... args)
    {
        return m_layerStack.PushLayer(Ref<Layer>(new T(std::forward<Args>(args)...)));
    }

    template<std::derived_from<Layer> T, typename... Args>
    FORCE_INLINE
    inline constexpr Layer* PushOverlay(Args&&... args)
    {
        return m_layerStack.PushOverlay(Ref<Layer>(new T(std::forward<Args>(args)...)));
    }

    FORCE_INLINE
    inline constexpr void PopLayer(const Ref<Layer>& layer)
    {
        return m_layerStack.PopLayer(layer);
    }
    
    FORCE_INLINE
    inline constexpr void PopOverlay(const Ref<Layer>& overlay)
    {
        return m_layerStack.PopOverlay(overlay);
    }

 protected:
    Application(const ApplicationSpecification& spec);

 private:
    static Application* s_instance;

    const ApplicationSpecification k_spec;

    LayerStack m_layerStack;

    Ref<Window> m_window;
    Ref<Renderer::Context> m_rendererContext;

    Scene* m_scene;

    bool m_running;
};