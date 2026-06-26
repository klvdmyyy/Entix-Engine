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

        size_t start, end;

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

    std::expected<void, String> StringCommandRunner::Run(StringView cmd)
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

        if (firstNonSpace == String::npos)
            return std::unexpected("Failed to run command: Can't find token start (maybe it's empty?)");

        auto firstTokenEnd = firstNonSpace;
        for (size_t i = firstNonSpace; i < cmd.size(); i++)
        {
          if (std::isspace(cmd[i]))
            break;

          firstTokenEnd++;
        }

        if (firstTokenEnd == String::npos)
            return std::unexpected("Failed to run command: Can't find token end");

        String cmdSearchStr = String(
            cmd.substr(firstNonSpace, firstTokenEnd - firstNonSpace));

        std::lock_guard<std::mutex> lock(m_sync);
        auto searchIt = m_commandMap.find(cmdSearchStr);
        if (searchIt == m_commandMap.end())
        {
            Debug::Error(LogCategory::Console, "Command not found: {}", cmdSearchStr);
          return std::unexpected(std::format(
              "Failed to run unexisting command: {}", cmdSearchStr));
        }

        auto args = String(cmd.substr(
            firstTokenEnd, cmd.size() - firstTokenEnd));

        (searchIt->second)(CommandArgs::Parse(args));

        return {};
    }

    std::expected<void, String> StringCommandRunner::AddCommand(CommandSpecification cmd, CommandCallback callback)
    {
        std::lock_guard<std::mutex> lock(m_sync);

        if (m_commandMap.contains(cmd.name))
        {
            return std::unexpected(std::format("Failed to add command. Command already exists: {}", cmd.name));
        }

        m_commandSpecMap.insert({cmd.name, cmd});
        m_commandMap.insert({m_commandSpecMap.at(cmd.name).name, callback});

        return {};
    }

    std::expected<void, String> StringCommandRunner::RemoveCommand(StringView cmd)
    {
        std::lock_guard<std::mutex> lock(m_sync);

        if(!m_commandMap.contains(cmd))
        {
            return std::unexpected(std::format("Failed to remove command. Command doesn't exists: {}", cmd));
        }

        m_commandMap.erase(cmd);
        m_commandSpecMap.erase(cmd);

        return {};
    }

    std::expected<CommandSpecification, String> StringCommandRunner::GetSpec(StringView cmd) const
    {
        if(!m_commandSpecMap.contains(cmd))
        {
            return std::unexpected(std::format("Failed to get command specification. Command doesn't exists: {}", cmd));
        }

        return m_commandSpecMap.at(cmd);
    }

    void StringCommandRunner::Each(std::function<void(const CommandSpecification& spec)> callback) const
    {
      for (auto &specIt : m_commandSpecMap)
        {
            (callback)(specIt.second);
        }
    }
}