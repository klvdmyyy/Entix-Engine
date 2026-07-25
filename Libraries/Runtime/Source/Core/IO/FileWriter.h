#pragma once

#include "Core/IO/Base.h"
#include "Core/String.h"

#include <filesystem>
#include <fstream>

namespace IO {
    class FileWriter : public Writer {
    public:
        FileWriter(const std::filesystem::path& filepath);
        ~FileWriter();

        size_t Write(const void* buffer, size_t size) override;
        void Flush() override { m_file.flush(); }

        const std::filesystem::path& GetPath() const { return k_filepath; }
    
    private:
        bool m_exists = false;
        std::fstream m_file;

        const std::filesystem::path k_filepath;
    };
}