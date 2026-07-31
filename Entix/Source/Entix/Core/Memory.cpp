#include "Entix/Core/Memory.h"

#include <cstdlib>

// NOLINTBEGIN
// void* operator new(size_t count)
// {
//     auto ptr = malloc(count);
//     return ptr;
// }

// void operator delete(void* ptr) noexcept
// {
//     free(ptr);
// }

// void operator delete(void* ptr, [[maybe_unused]] Entix::Usize sz) noexcept
// {
//     free(ptr);
// }
// NOLINTEND