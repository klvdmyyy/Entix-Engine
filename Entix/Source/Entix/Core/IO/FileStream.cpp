#include "Entix/Core/IO/FileStream.h"

#include "Entix/Core/Debug/Logger.h"
#include "Entix/Core/Globals.h"

namespace Entix::IO
{
    FileStream::FileStream(const std::filesystem::path& filepath, StreamMode mode)
        :  k_mode(mode), k_path(filepath)
    {
        EX_LOG(LogIO, Debug, "Creating FileStream object. Path: '{}' StreamMode: {}", filepath.string(), mode);
        auto iosmode = std::ios::binary | std::ios::ate;

        if(k_mode == StreamMode::Read)
            iosmode |= std::ios::in;
        
        if(k_mode == StreamMode::Write)
            iosmode |= std::ios::out;

        m_exists = std::filesystem::exists(filepath);
        
        if(!m_exists)
            EX_LOG(LogIO, Warning, "File doesn't exists: '{}'", k_path.string());

        m_file.open(filepath, iosmode);

        if(m_exists)
        {
            m_file.seekg(0, std::ios::end);
            m_size = m_file.tellg();
            m_file.seekg(0, std::ios::beg);
        }
    }

    FileStream::~FileStream()
    {
        Flush();
        m_file.close();
    }

    Result<void> FileStream::Write(ConstByteSpan src)
    {
        if(k_mode == StreamMode::Read)
            return Error("Can't write to file stream. Stream mode is Reading only!");
        
        m_file.write(reinterpret_cast<const char *>(src.data()), src.size());

        return {};
    }

    Result<void> FileStream::Read(ByteSpan dest)
    {
        if(!m_exists)
            return Error("File doesn't exists");
        
        if(k_mode == StreamMode::Write)
            return Error("Can't read from file stream. Stream mode is Writing only!");
        
        EX_LOG(LogIO, Trace, "Reading {} bytes from '{}'", dest.size(), k_path.string());
        m_file.read(reinterpret_cast<char *>(dest.data()), dest.size());

        return {};
    }

    Result<void> FileStream::Seek(Int64 offset, SeekOrigin origin)
    {
        if(!m_exists)
            return Error("File doesn't exists");
        
        switch(origin)
        {
            case SeekOrigin::Begin:
                m_file.seekg(offset, std::ios::beg);
                return {};
            
            case SeekOrigin::Current:
                m_file.seekg(offset, std::ios::cur);
                return {};
            
            case SeekOrigin::End:
                m_file.seekg(offset, std::ios::end);
                return {};
        }

        return Error("Unknown SeekOrigin value!");
    }

}