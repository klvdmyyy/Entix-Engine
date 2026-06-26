
#include "Core/Application.h"

#include "Core/Assert.h"

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
        m_context = Ref<Context>(Context::Create(m_window));
    }

    Application::~Application()
    {
        EventBus::Instance().RemoveListener(this);
    }

    void Application::Run(int argc, char** argv)
    {
        auto shader = Ref<Shader>(m_context->CreateShader(VBOX_SIMPLE_SHADER));

        auto vertex_array = Ref<VertexArray>(m_context->CreateVertexArray());

        float vertices[] = {
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f
        };

        auto vertex_buffer = Ref<VertexBuffer>(m_context->CreateVertexBuffer(vertices, sizeof(vertices)));

        BufferLayout layout = {
            { "a_Position", ShaderDataType::Float3 },
        };

        vertex_buffer->SetLayout(layout);

        uint32_t indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        auto index_buffer = Ref<IndexBuffer>(m_context->CreateIndexBuffer(indices, sizeof(indices)));

        vertex_array->AddVertexBuffer(vertex_buffer);
        vertex_array->SetIndexBuffer(index_buffer);

        ActionSystem::Instance().PushContext(ActionContext{"MoveForward", "MoveBackward", "MoveRight", "MoveLeft"});
        ActionSystem::Instance().PushContext(ActionContext{"MoveForward", "MoveBackward", "Jump"});

        ActionSystem::Instance().SetActionMap(ActionMap::LoadFromFile(VBOX_DEFAULT_ACTION_MAP));

        while(m_running)
        {
            EventBus::Instance().ProcessEvents();

            if(ActionSystem::Instance().IsPressed("MoveForward"))
                EX_LOG(Trace, LogCategory::WSI, "MoveForward is pressed!");

            if(ActionSystem::Instance().IsPressed("MoveBackward"))
                EX_LOG(Trace, LogCategory::WSI, "MoveBackward is pressed!");

            if(ActionSystem::Instance().IsPressed("MoveLeft"))
                EX_LOG(Trace, LogCategory::WSI, "MoveLeft is pressed!");

            if(ActionSystem::Instance().IsPressed("MoveRight"))
                EX_LOG(Trace, LogCategory::WSI, "MoveRight is pressed!");

            if(ActionSystem::Instance().IsPressed("Jump"))
                ActionSystem::Instance().PopContext();

            m_window->Update();

            m_context->BeginScene();

            m_context->SetClearColor(0.2f, 0.2f, 0.2f);
            m_context->Clear();

            m_context->Submit(shader, vertex_array);

            m_context->EndScene();

            m_context->Swap();

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
}