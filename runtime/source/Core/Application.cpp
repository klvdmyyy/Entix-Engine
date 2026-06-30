
#include "Core/Application.h"
#include "Core/Assert.h"

#include "Core/Resources/ResourceManager.h"

#include "WSI/ActionSystem.h"

#include <tracy/Tracy.hpp>

namespace ERUNTIME_NAMESPACE
{
    Application* Application::s_instance = nullptr;

    Application::Application(const ApplicationSpecification& spec)
        : k_spec(spec), m_running(true)
    {
        // Only one application instance for program.
        EX_ASSERT(!s_instance, "failed to create application. application instance already exists");

        // Setting up an application instance.
        s_instance = this;

        // Tracing macro
        ZoneScoped;

        // Initialization
        EventBus::Instance().AddListener(this);
        m_window = Ref<Window>(Window::Create(k_spec.windowSpec));
        m_rendererContext = Ref<Renderer::Context>(Renderer::Context::Create(m_window));
        m_guiContext = CreateScope<GUI::Context>(m_rendererContext);
        
        ResourceManager::Instance().SetRendererContext(m_rendererContext);
    }

    Application::~Application()
    {
        EventBus::Instance().RemoveListener(this);
    }

    void Application::Run(int argc, char** argv)
    {
        this->OnInit();

        ResourceManager::Instance().LoadShader(VBOX_SIMPLE_SHADER);

        auto vertex_array = Ref<Renderer::VertexArray>(m_rendererContext->CreateVertexArray());

        float vertices[] = {
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
        };

        auto vertex_buffer = Ref<Renderer::VertexBuffer>(m_rendererContext->CreateVertexBuffer(vertices, sizeof(vertices)));

        Renderer::BufferLayout layout = {
            { "a_Position", Renderer::ShaderDataType::Float3 },
        };

        vertex_buffer->SetLayout(layout);

        uint32_t indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        auto index_buffer = Ref<Renderer::IndexBuffer>(m_rendererContext->CreateIndexBuffer(indices, sizeof(indices)));

        vertex_array->AddVertexBuffer(vertex_buffer);
        vertex_array->SetIndexBuffer(index_buffer);

        while(m_running)
        {
            EventBus::Instance().ProcessEvents();

            ActionSystem::Instance().Update();

            this->OnTick();

            m_window->Update();

            m_guiContext->OnPreUpdate();

            m_rendererContext->BeginScene();

            m_rendererContext->SetClearColor(0.2f, 0.2f, 0.2f);
            m_rendererContext->Clear();

            m_rendererContext->Submit(ResourceManager::Instance().GetShader(VBOX_SIMPLE_SHADER), vertex_array);
            
            m_rendererContext->EndScene();

            m_guiContext->OnPostRender();

            m_rendererContext->Swap();

            FrameMark;
        }

        this->OnShutdown();
    }

    void Application::OnEvent(const Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowCloseEvent>([&](const WindowCloseEvent& e) {
            m_running = false;
        });
    }
}