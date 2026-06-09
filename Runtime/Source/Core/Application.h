#pragma once

#include <string>

#include "ERuntimeExport.h"
#include "Core/Window.h"

namespace Core {
	struct ApplicationSpecification
	{
		std::string Name;
		WindowSpecification WindowSpec;
	};

	class ERUNTIME_EXPORT Application
	{
	public:
		Application(const ApplicationSpecification& spec);
		~Application();

		void Run();
	};
}