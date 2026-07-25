#include "Core/IO/MemoryReader.h"

namespace IO
{
    void MemoryReader::Seek(Int64 offset, SeekOrigin origin)
    {
        switch(origin)
        {
            case SeekOrigin::Begin:
                m_cursor = offset;
                break;
            
            case SeekOrigin::Current:
                m_cursor += offset;
                break;
            
            case SeekOrigin::End:
                m_cursor = Size() + offset;
                break;
        }
    }

    size_t MemoryReader::Read(void* buffer, size_t size)
    {
        auto readSize = std::min(k_size, size);
        memcpy(buffer, static_cast<const uint8_t*>(k_buffer) + m_cursor, readSize);
        m_cursor += readSize;
        return readSize;
    }
}