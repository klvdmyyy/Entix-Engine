#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/IO/Stream.h"

#include <filesystem>
#include <fstream>

namespace Entix::IO
{
    class FileStream : public Stream
    {
    public:
        ENTIX_API FileStream(const std::filesystem::path& filepath, StreamMode mode);
        ENTIX_API ~FileStream();

        ENTIX_API Result<void> Write(ConstByteSpan src) override;
        ENTIX_API void Flush() override { m_file.flush(); }

        ENTIX_API Result<void> Read(ByteSpan dest) override;
        ENTIX_API Result<void> Seek(Int64 offset, SeekOrigin origin) override;
        ENTIX_API Usize Tell() const override { return m_exists ? static_cast<Usize>(m_file.tellg()) : 0Ui64; }
        ENTIX_API Usize Size() const override { return m_size; }

    private:
        const StreamMode k_mode;

        bool m_exists = false;
        Usize m_size = 0;
        const std::filesystem::path k_path;

        mutable std::fstream m_file;
    };
}