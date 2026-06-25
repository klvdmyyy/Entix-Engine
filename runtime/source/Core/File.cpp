#include "Core/File.h"

#include "Core/Debug/Log.h"

namespace ERUNTIME_NAMESPACE
{
    File::File(const std::filesystem::path& filepath)
        : k_Filepath(filepath)
    {
        m_File.open(filepath, std::ios::in | std::ios::binary);
    }

    File::~File()
    { m_File.close();
    }

    String File::ReadStr()
    {
      if (!m_File)
        return "";

        String result;

        m_File.seekg(0, std::ios::end);
        size_t size = m_File.tellg();
        if(size != -1)
        {
            result.resize(size);
            m_File.seekg(0, std::ios::beg);
            m_File.read(&result[0], size);
        }
        else
        {
            // EX_LOG(Error, LogCategory::IO, "Can't read file: {}", k_Filepath);
            
        }

        return result;
    }
}