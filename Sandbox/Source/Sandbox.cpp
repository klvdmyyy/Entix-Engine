#include <Entix/Core/EntryPoint.h>
#include <Entix/Core/Result.h>
#include <Entix/Core/String.h>
#include <Entix/Core/Memory.h>

#include <cstdlib>
#include <print>

namespace Entix
{
    int Main(int argc, char** argv)
    {
        int* ptr = static_cast<int*>(malloc(sizeof(int)));
        Result<void> r = Error("Hello, World!");
        std::println("{}", r.GetError().What());
        return 0;
    }
}