
#include "Core/Application.h"

#include "Core/Assert.h"

#include <tracy/Tracy.hpp>

namespace ERUNTIME_NAMESPACE
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification& spec)
        : m_Spec(spec), m_Running(true)
    {
        // Only one application instance for program.
        EX_ASSERT(!s_Instance, "failed to create application. application instance already exists");

        // Setting up an application instance.
        s_Instance = this;

        // Tracing macro
        ZoneScoped;

        // Initialization
        EventBus::Instance().AddListener(this);
        m_Window = Ref<Window>(Window::Create(m_Spec.WindowSpec));
        m_Context = Ref<Context>(Context::Create(m_Window));
    }

    Application::~Application()
    {
        EventBus::Instance().RemoveListener(this);
    }

    void Application::Run(int argc, char** argv)
    {
        auto shader = Ref<Shader>(m_Context->CreateShader(VBOX_SIMPLE_SHADER));

        auto vertex_array = Ref<VertexArray>(m_Context->CreateVertexArray());

        float vertices[] = {
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
        };

        auto vertex_buffer = Ref<VertexBuffer>(m_Context->CreateVertexBuffer(vertices, sizeof(vertices)));

        BufferLayout layout = {
            { "a_Position", ShaderDataType::Float3 },
        };

        vertex_buffer->SetLayout(layout);

        uint32_t indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        auto index_buffer = Ref<IndexBuffer>(m_Context->CreateIndexBuffer(indices, sizeof(indices)));

        vertex_array->AddVertexBuffer(vertex_buffer);
        vertex_array->SetIndexBuffer(index_buffer);

        while(m_Running)
        {
            EventBus::Instance().ProcessEvents();

            m_Window->Update();

            m_Context->BeginScene();

            m_Context->SetClearColor(0.2f, 0.2f, 0.2f);
            m_Context->Clear();

            m_Context->Submit(shader, vertex_array);

            m_Context->EndScene();

            m_Context->Swap();

            FrameMark;
        }
    }

    void Application::OnEvent(const Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowCloseEvent>([&](const WindowCloseEvent& e) {
            m_Running = false;
        });
    }
}