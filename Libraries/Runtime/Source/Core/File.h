// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/String.h"

#include <filesystem>
#include <fstream>

class ERUNTIME_API File {
 public:
 File(const std::filesystem::path& filepath);
 ~File();

 String ReadStr();

 private:
 std::fstream m_file;
 std::filesystem::path k_filepath;
};