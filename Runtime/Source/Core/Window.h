#pragma once

#include <cstdint>
#include <string>

#include "ERuntimeExport.h"
#include "Core/Memory.h"

#include "Core/Event.h"

namespace Core
{
	using WindowID = uint32_t;

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent(const WindowID id)
			: m_ID(id)
		{
		}

		inline const WindowID GetWindowID() const noexcept
		{
			return m_ID;
		}

		DEFINE_EVENT_TYPE(WindowCloseEvent);

	private:
		const WindowID m_ID;
	};

	struct WindowSpecification
	{
		uint32_t Width;
		uint32_t Height;
		std::string Title;
	};

	class ERUNTIME_EXPORT Window
	{
	public:
		virtual ~Window() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void OnUpdate() = 0;

		static Ref<Window> Create(const WindowSpecification& spec);
	};
}