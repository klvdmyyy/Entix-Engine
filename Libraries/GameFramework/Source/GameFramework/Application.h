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

    virtual void OnInit() { }
    virtual void OnTick() { }
    virtual void OnShutdown() { }

    void OnEvent(const Event& event) final;

    ApplicationSpecification GetSpec() const noexcept { return k_spec; }

    Window* GetWindow() const { return m_window.get(); }
    Renderer::Context* GetRendererContext() const { return m_rendererContext.get(); }
    Scene& GetCurrentScene() const { return *m_scene; }

 protected:
    Application(const ApplicationSpecification& spec);

 private:
    static Application* s_instance;

    const ApplicationSpecification k_spec;

    Ref<Window> m_window;
    Ref<Renderer::Context> m_rendererContext;

    Scene* m_scene;

    bool m_running;
};