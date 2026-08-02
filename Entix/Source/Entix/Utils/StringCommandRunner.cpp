#include "Entix/Utils/StringCommandRunner.h"

namespace Entix
{
    StringCommandRunner& StringCommandRunner::Instance()
    {
        static StringCommandRunner s_instance;
        return s_instance;
    }
}