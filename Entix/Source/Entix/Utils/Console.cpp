#include "Entix/Utils/Console.h"

namespace Entix
{
    void Console::ExecuteCommand(const String& cmdLine)
    {
        // Split string to array of args and execute this overload
        ExecuteCommand(SplitString(cmdLine));
    }

    void Console::ExecuteCommand(const std::vector<String>& args)
    {
        if(args.empty()) return;

        auto cmd = args[0];

        auto cVarIt = m_cVarMap.find(cmd);

        if(cVarIt != m_cVarMap.end())
        {
            if(args.size() < 2)
            {
                // TODO Print value of CVar
                return;
            }

            auto value = args[1];
            SetCVarInternal(cmd, value);
            return;
        }

        // TODO Command execution
    }

    void Console::SetCVarInternal(const String& name, const String& value)
    {
        std::unique_lock lock(m_sync);

        m_cVarMap[name] = value;
    }

    String Console::GetCVarInternal(const String& name)
    {
        auto it = m_cVarMap.find(name);

        return it == m_cVarMap.end() ? "" : it->second;
    }

    // -----------------------------------
    // Simple split function for Console
    // commands.
    // -----------------------------------
    std::vector<String> Console::SplitString(const String& line)
    {
        std::vector<String> args;

        Usize idx = 0;
        while(idx < line.size())
        {
            String arg;
            for(Usize i = idx; i < line.size(); i++)
            {
                if(std::isspace(line[i]))
                    break;

                arg.push_back(line[i]);
            }
            args.push_back(arg);
        }

        return args;
    }
}
