#include "Entix/Core/Memory.h"

#include <cstdlib>

// NOLINTBEGIN
void* operator new(size_t count)
{
    auto ptr = malloc(count);
    return ptr;
}

void operator delete(void* ptr)
{
    free(ptr);
}
// NOLINTEND