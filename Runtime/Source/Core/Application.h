#pragma once

#include <string>

#include "ERuntimeExport.h"

#include "Core/Window.h"
#include "Core/EventBus.h"

namespace Core
{
	struct ApplicationSpecification
	{
		std::string Name;
		std::string Description;
		WindowSpecification WindowSpec;
	};

	class ERUNTIME_EXPORT Application : EventListener
	{
	public:
		Application(const ApplicationSpecification& spec, int argc, char** argv);
		~Application();

		void Run();

		void OnEvent(const Event& event) final;

	private:
		/* This two methods are implemented in Impl section */
		void Init();
		void Shutdown();

		bool m_Running;

		Ref<Window> m_Window;
	};
}