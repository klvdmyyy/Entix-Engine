#pragma once

#include "Core/Base.h"
#include "Core/String.h"
#include "Core/Types.h"
#include "Core/Assert.h"

#include "Core/IO/Writer.h"

#include <expected>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <vector>

namespace ERUNTIME_NAMESPACE
{
    // ---------------------------------------------------------------------
    // Описание комманды. Используется для комманды `help`
    // ---------------------------------------------------------------------
    struct CommandSpecification
    {
        String name;
        String description;
    };

    class ERUNTIME_API CommandArgs
    {
    public:
        [[nodiscard]]
        static CommandArgs Parse(StringView str);

        [[nodiscard]]
        std::size_t Count() const;

        StringView Get(std::size_t index) const
        {
            EX_ASSERT(index < m_args.size(), "Incorrect argument index");
            return m_args.at(index);
        }

    private:
        CommandArgs(const std::vector<StringView>& args);

        std::vector<StringView> m_args;
    };

    using CommandCallback = std::function<void(const CommandArgs&, IO::Writer&)>;

    class ERUNTIME_API StringCommandRunner
    {
    public:
        [[nodiscard]]
        static StringCommandRunner& Instance();

        void Run(StringView cmd, IO::Writer& writer);

        bool AddCommand(CommandSpecification cmd, CommandCallback callback);

        bool RemoveCommand(StringView cmd);

        std::vector<String> GetSuggestions(const String& prefix) const;
        
        [[nodiscard]]
        std::expected<CommandSpecification, String> GetSpec(StringView cmd) const;

        void Each(std::function<void(const CommandSpecification& spec)>) const;

    private:
        StringCommandRunner() = default;

        std::unordered_map<StringView, CommandCallback> m_commandMap;
        std::unordered_map<String, CommandSpecification> m_commandSpecMap;
        std::mutex m_sync;
    };
}