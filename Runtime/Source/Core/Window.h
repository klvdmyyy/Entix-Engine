#pragma once

#include <cstdint>
#include <string>

namespace Core
{
	struct WindowSpecification
	{
		uint32_t Width;
		uint32_t Height;
		std::string Title;
	};

	class Window
	{
	public:
		virtual ~Window() = default;
	};
}