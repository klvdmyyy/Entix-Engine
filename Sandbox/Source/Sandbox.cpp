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
        Result<Scope<String>, int> a = CreateScope<String>("Hello, World!");
        _ASSERT(a.IsSuccess());
        Scope<String> s = a.Unwrap();
        return 0;
    }
}