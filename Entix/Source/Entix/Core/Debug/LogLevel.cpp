#include "LogLevel.h"

namespace Entix
{
    String LogLevelToString(LogLevel level)
    {
#define ADD_LEVEL(X) case LogLevel::X: return #X;
        switch(level)
        {
            ADD_LEVEL(Trace);
            ADD_LEVEL(Debug);
            ADD_LEVEL(Info);
            ADD_LEVEL(Warning);
            ADD_LEVEL(Error);
            ADD_LEVEL(Fatal);

            default:
                return "Unknown";
        }
#undef ADD_LEVEL 
    }
}