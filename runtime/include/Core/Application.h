// -*- mode: c++; -*-
#pragma once

#include "Core/String.h"
#include "Core/Memory.h"
#include "Core/EventSystem.h"

#include "Core/Debug/Log.h"
#include "Core/StringCommandRunner.h"

#include "WSI/Window.h"

#include "Renderer/Context.h"

namespace ERUNTIME_NAMESPACE
{
    struct ApplicationSpecification
    {
        String Name;
        String Description;
        WindowSpecification WindowSpec;
    };

    class ERUNTIME_API Application : EventListener
    {
    public:
        Application(const ApplicationSpecification& spec);
        ~Application();

        static Application& Instance() { return *s_Instance; }

        void Run(int argc, char** argv);

        void OnEvent(const Event& event) final;

    private:
        static Application* s_Instance;

        ApplicationSpecification m_Spec;

        Ref<Window> m_Window;
        Ref<Context> m_Context;

        bool m_Running;
    };
}