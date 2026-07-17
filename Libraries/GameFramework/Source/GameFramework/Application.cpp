#include "GameFramework/Application.h"

#include "Core/Assert.h"
#include "Core/Time.h"

#include "Input/ActionSystem.h"

#include <tracy/Tracy.hpp>

static Application* g_instance = nullptr;

Application& Application::Get()
{
    return *g_instance;
}

Application::Application(const ApplicationSpecification& spec)
    : k_spec(spec), m_running(true), m_scene(nullptr)
{
    // Only one application instance for program.
    EX_ASSERT(!g_instance, "failed to create application. application instance already exists");

    // Setting up an application instance.
    g_instance = this;

    // Tracing macro
    ZoneScoped;

    // Initialization
    EventBus::Instance().SetImmediateMode(false);
    EventBus::Instance().AddListener(this);
    m_window = Ref<Window>(Window::Create(k_spec.windowSpec));
    m_rendererContext = Ref<Renderer::Context>(Renderer::Context::Create(m_window));

    m_scene = new Scene(m_rendererContext);
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
    Uint32 previousTicks = Time::GetTicks();
    
    while(m_running) {
        Uint32 currentTicks = Time::GetTicks();
        Timestep deltaTime = (currentTicks - previousTicks) / 1000.0f;

        EventBus::Instance().ProcessEvents();
        ActionSystem::Instance().Update();

        m_window->Update();

        for(auto& layer : m_layerStack) {
            layer->OnTick(deltaTime);
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

        previousTicks = currentTicks;
        
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