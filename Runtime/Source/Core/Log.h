#pragma once

#include "ERuntimeExport.h"
#include "Core/Memory.h"

#include <cstddef>
#include <cstdlib>
#include <deque>
#include <format>

#include <spdlog/spdlog.h>

namespace Core
{
	class ERUNTIME_EXPORT Log
	{
	public:
		static constexpr size_t MAX_ITEM_COUNT = 1000;

		struct Item
		{
			spdlog::level::level_enum Level;
			enum
			{
				Core,
				Client,
			} Source;

			std::string Message;
		};

		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger()
		{
			return s_CoreLogger;
		}

		static Ref<spdlog::logger>& GetClientLogger()
		{
			return s_ClientLogger;
		}

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;

		static std::deque<Item> s_CoreItems;
		static std::deque<Item> s_ClientItems;
	};
}

#define E_TRACE(...) ::Core::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define E_INFO(...) ::Core::Log::GetCoreLogger()->info(__VA_ARGS__)
#define E_WARN(...) ::Core::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define E_ERROR(...) ::Core::Log::GetCoreLogger()->error(__VA_ARGS__)
#define E_CRITICAL(...) ::Core::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define E_ASSERT(EXPR, ...) do { if(!(EXPR)) { E_CRITICAL("Assertion failed: '{0}'. Message: {1}", #EXPR, std::format(__VA_ARGS__)); std::exit(1); } } while(0)