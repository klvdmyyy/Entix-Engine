// -*- mode: c++; -*-
#pragma once

#include "Core/IO/Base.h"

#include "Core/Memory.h"

#define DEFINE_READER_DECORATOR(TYPE)           \
    public:                                     \
    TYPE(Scope<Reader> inner)                   \
    : ReaderDecorator(inner.release())          \
    {                                           \
    }                                           \
                                                \
    static TYPE CreateNonOwned(Reader& inner)   \
    {                                           \
        return std::move(TYPE(&inner, false));  \
    }                                           \
                                                \
private:                                        \
 TYPE(Reader* writer, bool owns)                \
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
    static TYPE CreateNonOwned(Writer& inner)   \
    {                                           \
        return std::move(TYPE(&inner, false));  \
    }                                           \
                                                \
private:                                        \
 TYPE(Writer* writer, bool owns)                \
     : WriterDecorator(inner, owns)             \
 {                                              \
 }

namespace IO {
    class TextWriter : public WriterDecorator {
        DEFINE_WRITER_DECORATOR(TextWriter);
        
    public:
        void Write(const String& str)
        {
            const auto writtenSize = Write(static_cast<const void*>(&str[0]), str.size());
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
            auto readedSize = m_inner->Read(result.data(), size);
            EX_ASSERT(readedSize == size, "Failed to read text data!");
            return result;
        }
    };
}