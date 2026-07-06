// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/IO/Reader.h"

#include <filesystem>
#include <fstream>

class FileReader : public IO::Reader {
public:
    FileReader(const std::filesystem::path& filepath);
    ~FileReader();

    String ReadStr() final;

    const std::filesystem::path& GetFilepath() const noexcept { return k_filepath; }

private:
    bool m_exists;
    std::ifstream m_file;
    const std::filesystem::path k_filepath;
};