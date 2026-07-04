#include "Core/Debug/LogLevel.h"

#include "Core/Assert.h"

const StringView LogLevelToString(const LogLevel& level)
{
#define ADD_LEVEL(X) case LogLevel::X: return #X
    switch(level)
        {
            ADD_LEVEL(Trace);
            ADD_LEVEL(Info);
            ADD_LEVEL(Warning);
            ADD_LEVEL(Error);
            ADD_LEVEL(Critical);
        }
#undef ADD_LEVEL

    EX_ASSERT(false, "Unknown log level provided: {}", static_cast<int>(level));
}
