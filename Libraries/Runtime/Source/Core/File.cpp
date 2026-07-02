#include "Core/File.h"

#include "Core/Debug/Log.h"

File::File(const std::filesystem::path& filepath)
    : k_filepath(filepath)
{
    m_file.open(filepath, std::ios::in | std::ios::binary);
}

File::~File()
{ m_file.close();
}

String File::ReadStr()
{
    if (!m_file)
        return "";

    String result;

    m_file.seekg(0, std::ios::end);
    size_t size = m_file.tellg();
    if(size != -1)
        {
            result.resize(size);
            m_file.seekg(0, std::ios::beg);
            m_file.read(&result[0], size);
        }
    else
        {
            Debug::Error(LogCategory::IO, "Can't read file: {}", String(k_filepath));
        }

    return result;
}