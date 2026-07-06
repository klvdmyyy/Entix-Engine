#include "GameFramework/Application.h"

#include "Core/Assert.h"

#include "Core/Resources/ResourceManager.h"

#include "WSI/ActionSystem.h"

#include <tracy/Tracy.hpp>

Application* Application::s_instance = nullptr;

Application::Application(const ApplicationSpecification& spec)
    : k_spec(spec), m_running(true), m_scene(nullptr)
{
    // Only one application instance for program.
    EX_ASSERT(!s_instance, "failed to create application. application instance already exists");

    // Setting up an application instance.
    s_instance = this;

    // Tracing macro
    ZoneScoped;

    // Initialization
    EventBus::Instance().SetImmediateMode(false);
    EventBus::Instance().AddListener(this);
    m_window = Ref<Window>(Window::Create(k_spec.windowSpec));
    m_rendererContext = Ref<Renderer::Context>(Renderer::Context::Create(m_window));

    m_scene = new Scene(m_rendererContext);
        
    ResourceManager::Instance().SetRendererContext(m_rendererContext);
}

Application::~Application()
{
    if(m_scene) {
        delete m_scene;
        m_scene = nullptr;
    }

    EventBus::Instance().RemoveListener(this);
}

void Application::Run(int argc, char** argv)
{
    while(m_running) {
        EventBus::Instance().ProcessEvents();
            
        ActionSystem::Instance().Update();

        m_window->Update();

        for(auto& layer : m_layerStack) {
            layer->OnTick(0.0f);
        }

        for(auto& layer : m_layerStack) {
            layer->OnPreRender();
        }
        
        for(auto& layer : m_layerStack) {
            layer->OnRender();
        }
        
        for(auto& layer : m_layerStack) {
            layer->OnPostRender();
        }

        m_rendererContext->Swap();

        FrameMark;
    }
}

void Application::OnEvent(const Event& event)
{
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<WindowCloseEvent>([&](const WindowCloseEvent& e) {
        m_running = false;
    });
}