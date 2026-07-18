#include "Core/Debug/LogFormatters.h"

DefaultFormatter* DefaultFormatter::Instance()
{
    static DefaultFormatter s_instance;
    return &s_instance;
}

ConsoleFormatter* ConsoleFormatter::Instance()
{
    static ConsoleFormatter s_instance;
    return &s_instance;
}