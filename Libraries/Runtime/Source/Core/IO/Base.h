// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/Types.h"
#include "Core/Memory.h"

#define DEFINE_READER_DECORATOR(TYPE)           \
    public:                                     \
    TYPE(Scope<::IO::Reader> inner)                   \
    : ::IO::ReaderDecorator(inner.release())          \
    {                                           \
    }                                           \
                                                \
    FORCE_INLINE inline static TYPE CreateNonOwned(::IO::Reader& inner)   \
    {                                           \
        return TYPE(&inner, false);             \
    }                                           \
                                                \
private:                                        \
 TYPE(::IO::Reader* inner, bool owns)                \
     : ::IO::ReaderDecorator(inner, owns)             \
 {                                              \
 }

#define DEFINE_WRITER_DECORATOR(TYPE)           \
    public:                                     \
    TYPE(Scope<::IO::Writer> inner)                   \
    : ::IO::WriterDecorator(inner.release())          \
    {                                           \
    }                                           \
                                                \
    FORCE_INLINE inline static TYPE CreateNonOwned(::IO::Writer& inner)   \
    {                                           \
        return TYPE(&inner, false);             \
    }                                           \
                                                \
private:                                        \
 TYPE(::IO::Writer* inner, bool owns)                \
     : ::IO::WriterDecorator(inner, owns)             \
 {                                              \
 }

namespace IO {
    enum class SeekOrigin {
        Begin,
        Current,
        End,
    };
    
    class Reader {
    public:
        virtual ~Reader() = default;

        virtual size_t Read(void* buffer, size_t size) = 0;
        virtual void Seek(Int64 offset, SeekOrigin origin) = 0;
        virtual size_t Tell() const = 0;
        virtual size_t Size() const = 0;
    };

    class Writer {
    public:
        virtual ~Writer()
        {
            this->Flush();
        }

        virtual size_t Write(const void* buffer, size_t size) = 0;
        virtual void Flush() {};
    };

    class ReaderDecorator : public Reader {
    public:
        explicit ReaderDecorator(Reader* inner, bool takeOwnership = true)
            : m_inner(inner), m_ownsInner(takeOwnership)
        {
        }

        virtual ~ReaderDecorator()
        {
            if(m_ownsInner) delete m_inner;
        }

        size_t Read(void* buffer, size_t size) override { return m_inner->Read(buffer, size); }
        void Seek(Int64 offset, SeekOrigin origin) override { return m_inner->Seek(offset, origin); }
        size_t Tell() const override { return m_inner->Tell(); }
        size_t Size() const override { return m_inner->Size(); }

    protected:
        Reader* m_inner;
        bool m_ownsInner;
    };

    class WriterDecorator : public Writer {
    public:
        explicit WriterDecorator(Writer* inner, bool takeOwnership = true)
            : m_inner(inner), m_ownsInner(takeOwnership)
        {
        }

        virtual ~WriterDecorator()
        {
            if(m_ownsInner) delete m_inner;
        }

        size_t Write(const void* buffer, size_t size) override { return m_inner->Write(buffer, size); }
        void Flush() override { return m_inner->Flush(); }

    protected:
        Writer* m_inner;
        bool m_ownsInner;
    };
}