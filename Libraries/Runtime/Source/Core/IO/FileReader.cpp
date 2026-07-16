#include "Core/IO/FileReader.h"

#include "Core/Assert.h"

namespace IO {
FileReader::FileReader(const std::filesystem::path &filepath) : k_filepath(filepath)
{
  m_exists = std::filesystem::exists(filepath);
  EX_ASSERT(m_exists,
            "Failed to create FileReader object for '{}'. File doesn't exists!",
            k_filepath.string());
  m_file.open(k_filepath, std::ios::in | std::ios::binary);

  m_file.seekg(0, std::ios::end);
  m_fileSize = m_file.tellg();
  m_file.seekg(0, std::ios::beg);
}

FileReader::~FileReader()
{ m_file.close(); }

void FileReader::Seek(Int64 offset, SeekOrigin origin)
{
    switch (origin)
    { case SeekOrigin::Begin:
      m_file.seekg(offset, std::ios::beg);
      break;

      case SeekOrigin::Current:
      m_file.seekg(offset, std::ios::cur);
        break;

        case SeekOrigin::End:
        m_file.seekg(offset, std::ios::end);
        break;

        default:
          EX_ASSERT(false, "Unknown SeekOrigin value in FileReader!");
    }
}

size_t FileReader::Read(void* buffer, size_t size)
{ m_file.read(static_cast<char*>(buffer), size - Tell()); return size; }
}