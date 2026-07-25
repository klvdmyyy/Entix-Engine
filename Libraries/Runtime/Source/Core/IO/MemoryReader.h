#pragma once

#include "Core/IO/Base.h"

namespace IO {
    class MemoryReader : public Reader {
    public:
        MemoryReader(const void* buffer, size_t size)
            : k_buffer(buffer), k_size(size)
        {
        }

        size_t Read(void* buffer, size_t size) final;
        void Seek(Int64 offset, SeekOrigin origin) final;

        size_t Tell() const final { return m_cursor; };
        size_t Size() const final { return k_size; };

    private:
        const void* k_buffer;
        const size_t k_size;
        
        size_t m_cursor = 0;
    };
}