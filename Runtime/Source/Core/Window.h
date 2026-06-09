#pragma once

#include <cstdint>
#include <string>

#include "Core/Memory.h"

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

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		static Ref<Window> Create(const WindowSpecification& spec);
	};
}