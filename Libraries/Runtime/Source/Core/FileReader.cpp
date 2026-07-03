#include "Core/FileReader.h"

FileReader::FileReader(const std::filesystem::path& filepath)
    : k_filepath(filepath), m_exists(false)
{
    m_exists = std::filesystem::exists(filepath);
    EX_ASSERT(m_exists, "Failed to create FileReader for '{}'. File doesn't exists", filepath);
    m_file.open(filepath, std::ios::in | std::ios::binary);
}

FileReader::~FileReader()
{
    m_file.close();
}

String FileReader::ReadStr()
{
    if(!m_file)
        return String();

    String result;

    m_file.seekg(0, std::ios::end);
    std::size_t size = m_file.tellg();

    if(size != -1) {
        result.resize(size);
        m_file.seekg(0, std::ios::beg);
        m_file.read(&result[0], size);
    } else {
        Debug::Error(LogCategory::IO, "Can't read file: {}", k_filepath);
    }

    return result;
}