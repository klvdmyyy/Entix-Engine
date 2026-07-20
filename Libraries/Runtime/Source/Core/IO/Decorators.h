// -*- mode: c++; -*-
#pragma once

#include "Core/IO/Base.h"
#include "Core/Types.h"
#include "Core/Memory.h"

#include <vector>

#define DEFINE_READER_DECORATOR(TYPE)           \
    public:                                     \
    TYPE(Scope<Reader> inner)                   \
    : ReaderDecorator(inner.release())          \
    {                                           \
    }                                           \
                                                \
    FORCE_INLINE inline static TYPE CreateNonOwned(Reader& inner)   \
    {                                           \
        return TYPE(&inner, false);             \
    }                                           \
                                                \
private:                                        \
 TYPE(Reader* inner, bool owns)                \
     : ReaderDecorator(inner, owns)             \
 {                                              \
 }

#define DEFINE_WRITER_DECORATOR(TYPE)           \
    public:                                     \
    TYPE(Scope<Writer> inner)                   \
    : WriterDecorator(inner.release())          \
    {                                           \
    }                                           \
                                                \
    FORCE_INLINE inline static TYPE CreateNonOwned(Writer& inner)   \
    {                                           \
        return TYPE(&inner, false);             \
    }                                           \
                                                \
private:                                        \
 TYPE(Writer* inner, bool owns)                \
     : WriterDecorator(inner, owns)             \
 {                                              \
 }

namespace IO {
    class TextWriter : public WriterDecorator {
        DEFINE_WRITER_DECORATOR(TextWriter);
        
    public:
        void Write(const String& str)
        {
            const auto writtenSize = m_inner->Write(str.data(), str.size());
            EX_ASSERT(writtenSize == str.size(), "Failed to write text data.");
        }

        void WriteLine(const String& str)
        {
            Write(str);
            Write("\n");
        }

        template<typename... Args>
        FORCE_INLINE
        inline void WriteFmt(std::format_string<Args...> fmt, Args&&... args)
        {
            Write(std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        FORCE_INLINE
        inline void WriteLineFmt(std::format_string<Args...> fmt, Args&&... args)
        {
            WriteLine(std::format(fmt, std::forward<Args>(args)...));
        }
    };

    class TextReader : public ReaderDecorator {
        DEFINE_READER_DECORATOR(TextReader);
        
    public:
        String Read(size_t size)
        {
            String result(size, '\0');
            auto readedSize = m_inner->Read(result.data(), size);
            EX_ASSERT(readedSize == size, "Failed to read text data!");
            return result;
        }

        String ReadLine()
        {
            String result;
            char ch;
            while(m_inner->Read(&ch, 1) == 1 && ch != '\n') {
                result += ch;
            }
            return result;
        }

        String ReadAll()
        {
            auto size = m_inner->Size() - m_inner->Tell();
            String result(size, '\0');
            auto readedSize = m_inner->Read(&result[0], size);
            EX_ASSERT(readedSize == size, "Failed to read text data!");
            return result;
        }
    };

    class BufferedWriter : public WriterDecorator {
        DEFINE_WRITER_DECORATOR(BufferedWriter);

    public:
        virtual ~BufferedWriter() override
        {
            this->Flush();
        }

        size_t Write(const void* data, size_t size) override
        {
            const char* bytes = static_cast<const char*>(data);
            size_t totalWritten = 0;

            m_buffer.resize(m_buffer.size() + size);

            std::memcpy(m_buffer.data() + m_cursor, bytes + totalWritten, size);
            m_cursor += size;
            totalWritten += size;

            return totalWritten;
        }

        void Flush() override
        {
            if(m_cursor > 0) {
                FlushBuffer();
                m_inner->Flush();
            }
        }

    private:
        void FlushBuffer()
        {
            NullTerminate();
            
            size_t written = m_inner->Write(m_buffer.data(), m_buffer.size());
            if(written < m_cursor) {
                size_t remaining = m_cursor - written;
                std::memmove(m_buffer.data(), m_buffer.data() + written, remaining);
                m_cursor = remaining;
            } else {
                m_cursor = 0;
            }
        }

        void NullTerminate()
        {
            m_buffer.push_back('\0');
            m_cursor += 1;
        }

        std::vector<char> m_buffer;
        size_t m_cursor = 0;
    };
}