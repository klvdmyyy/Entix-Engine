// -*- mode: c++; -*-
#pragma once

#include "Core/IO/Base.h"
#include "Core/String.h"

#include <filesystem>
#include <fstream>

namespace IO {
    class FileReader : public Reader {
    public:
        FileReader(const std::filesystem::path& filepath);
        ~FileReader();

        size_t Read(void* buffer, size_t size) final;
        void Seek(Int64 offset, SeekOrigin origin) final;
        size_t Tell() const final { return m_file.tellg(); };
        size_t Size() const final { return m_fileSize; };

        const std::filesystem::path& GetPath() const { return k_filepath; }

    private:
        mutable std::fstream m_file;
        
        size_t m_fileSize;
        const std::filesystem::path k_filepath;
        bool m_exists = false;
    };
}