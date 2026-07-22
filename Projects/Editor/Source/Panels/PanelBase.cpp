#include "PanelBase.h"

#include "Core/StringCommandRunner.h"

#include "Core/IO/Decorators.h"

#include <algorithm>
#include <cctype>
#include <format>

EditorPanelBase::EditorPanelBase(const String& name)
{
    String windowName = name;
    std::transform(windowName.begin(), windowName.end(), windowName.begin(), [](unsigned char c) {
        if(c == ' ') return '_';
        else return static_cast<char>(std::tolower(c));
    });

    m_cmdName = std::format("e_show_{}", windowName);

    StringCommandRunner::Instance().AddCommand(
        CommandSpecification{
            .name = m_cmdName,
            .description = "Opens the editor window."
        },
        [&](const CommandArgs& args, IO::Writer& rawWriter) {
            auto writer = IO::TextWriter::CreateNonOwned(rawWriter);

            if(args.Count() != 1) {
                writer.WriteFmt("Usage: {} [0/1]", m_cmdName);
                return;
            }

            if(args.Get(0) == "0") {
                open = false;
            } else {
                open = true;
            }
        }
    );
}