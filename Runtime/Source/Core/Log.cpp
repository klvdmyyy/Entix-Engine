#include "Core/Log.h"

#include <spdlog/sinks/callback_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Core
{
	Ref<spdlog::logger> Log::s_CoreLogger;
	Ref<spdlog::logger> Log::s_ClientLogger;

	std::deque<Log::Item> Log::s_CoreItems{ };
	std::deque<Log::Item> Log::s_ClientItems{ };


    void Log::Init()
    {

        spdlog::set_pattern("%^[%T] %n: %v%$");

        auto coreStdoutSink = CreateRef<spdlog::sinks::stdout_color_sink_mt>();
        auto coreItemSink = CreateRef<spdlog::sinks::callback_sink_mt>([&](const spdlog::details::log_msg& msg) {
            if (s_CoreItems.size() > MAX_ITEM_COUNT)
                s_CoreItems.pop_front();

            Item item;
            item.Level = msg.level;
            item.Source = Item::Core;
            item.Message = std::string{ msg.payload.begin(), msg.payload.end() };
            s_CoreItems.push_back(item);
            });

        s_CoreLogger = CreateRef<spdlog::logger>(spdlog::logger("Entix", { coreStdoutSink, coreItemSink }));
        s_CoreLogger->set_level(spdlog::level::trace);

        auto clientStdoutSink = CreateRef<spdlog::sinks::stdout_color_sink_mt>();
        auto clientItemSink = CreateRef<spdlog::sinks::callback_sink_mt>([&](const spdlog::details::log_msg& msg) {
            if (s_ClientItems.size() > MAX_ITEM_COUNT)
                s_ClientItems.pop_front();

            Item item;
            item.Level = msg.level;
            item.Source = Item::Client;
            item.Message = std::string{ msg.payload.begin(), msg.payload.end() };
            s_ClientItems.push_back(item);
            });

        s_ClientLogger = CreateRef<spdlog::logger>(spdlog::logger("Client", { clientStdoutSink, clientItemSink }));
        s_ClientLogger->set_level(spdlog::level::trace);
    }
}