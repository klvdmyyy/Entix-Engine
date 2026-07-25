#include "Core/IO/FileWriter.h"

#include "Core/Assert.h"

namespace IO {
    FileWriter::FileWriter(const std::filesystem::path& filepath)
        : k_filepath(filepath)
    {
        m_file.open(k_filepath, std::ios::out | std::ios::binary | std::ios::trunc);

        EX_ASSERT_EXPR(m_file.is_open());
    }

    FileWriter::~FileWriter()
    {
        m_file.close();
    }

    size_t FileWriter::Write(const void* buffer, size_t size)
    {
        m_file.write(static_cast<const char*>(buffer), size);
        return size;
    }
}