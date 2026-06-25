// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/String.h"

#include <filesystem>
#include <fstream>

namespace ERUNTIME_NAMESPACE
{
    class ERUNTIME_API File
    {
    public:
        File(const std::filesystem::path& filepath);
        ~File();

        String ReadStr();

    private:
        std::fstream m_File;
        const std::filesystem::path k_Filepath;
    };
}