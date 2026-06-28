#include "Core/StringCommandRunner.h"

#include "Core/Debug/Log.h"

#include <cctype>
#include <format>

namespace ERUNTIME_NAMESPACE
{
    /*
     Command Args
     */
    
    CommandArgs::CommandArgs(const std::vector<StringView>& args)
        : m_args(args)
    {
    }

    CommandArgs CommandArgs::Parse(StringView str)
    {
        std::vector<StringView> args;

        std::size_t start, end;

        start = end = 0;

        while((start = str.find_first_not_of(' ', end)) != std::string::npos)
        {
            end = str.find(' ', start);
            args.push_back(str.substr(start, end - start));
        }

        return CommandArgs(args);
    }

    std::size_t CommandArgs::Count() const
    {
        return m_args.size();
    }

    /*
     String Command Runner
     */

    StringCommandRunner& StringCommandRunner::Instance()
    {
        static StringCommandRunner s_runner;
        return s_runner;
    }

    void StringCommandRunner::Run(StringView cmd, IO::Writer& writer)
    {
        auto firstNonSpace = String::npos;

        for(size_t i = 0; i < cmd.size(); i++)
        {
            if(std::isspace(cmd[i]) == 0)
            {
            firstNonSpace = i;
                break;
            }
        }

        if (firstNonSpace == String::npos) {
            Debug::Error(LogCategory::Console, "Failed to run command: Can't find token start (maybe it's empty?)");
            return;
        }

        auto firstTokenEnd = firstNonSpace;
        for (size_t i = firstNonSpace; i < cmd.size(); i++)
        {
          if (std::isspace(cmd[i]))
            break;

          firstTokenEnd++;
        }

        if (firstTokenEnd == String::npos) {
            Debug::Error(LogCategory::Console, "Failed to run command: Can't find token end");
            return;
        }

        String cmdSearchStr = String(
            cmd.substr(firstNonSpace, firstTokenEnd - firstNonSpace));

        std::lock_guard<std::mutex> lock(m_sync);
        auto searchIt = m_commandMap.find(cmdSearchStr);
        if (searchIt == m_commandMap.end())
        {
            Debug::Error(LogCategory::Console, "Command not found: {}", cmdSearchStr);
            return;
        }

        auto args = String(cmd.substr(
            firstTokenEnd, cmd.size() - firstTokenEnd));

        (searchIt->second)(CommandArgs::Parse(args), writer);
    }

    bool StringCommandRunner::AddCommand(CommandSpecification cmd, CommandCallback callback)
    {
        std::lock_guard<std::mutex> lock(m_sync);

        if (m_commandMap.contains(cmd.name))
        {
            Debug::Error(LogCategory::Console, "Failed to add command. Command already exists: {}", cmd.name);
            return false;
        }

        m_commandSpecMap.insert({cmd.name, cmd});
        m_commandMap.insert({m_commandSpecMap.at(cmd.name).name, callback});

        return true;
    }

    bool StringCommandRunner::RemoveCommand(StringView cmd)
    {
        std::lock_guard<std::mutex> lock(m_sync);

        if(!m_commandMap.contains(cmd))
        {
            Debug::Error(LogCategory::Console, "Failed to remove command. Command doesn't exists: {}", cmd);
            return false;
        }

        m_commandMap.erase(cmd);
        m_commandSpecMap.erase(String(cmd));

        return true;
    }

    std::vector<String> StringCommandRunner::GetSuggestions(const String& prefix) const
    {
        std::vector<String> suggestions;
        for(const auto& [name, _] : m_commandSpecMap) {
            if(name.find(prefix) == 0) {
                suggestions.push_back(name);
            }
        }
        return suggestions;
    }

    std::expected<CommandSpecification, String> StringCommandRunner::GetSpec(StringView cmd) const
    {
        if(!m_commandSpecMap.contains(String(cmd)))
        {
            return std::unexpected(std::format("Failed to get command specification. Command doesn't exists: '{}'", cmd));
        }

        return m_commandSpecMap.at(String(cmd));
    }

    void StringCommandRunner::Each(std::function<void(const CommandSpecification& spec)> callback) const
    {
      for (auto &specIt : m_commandSpecMap)
        {
            (callback)(specIt.second);
        }
    }
}