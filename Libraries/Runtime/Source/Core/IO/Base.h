// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"
#include "Core/Types.h"

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
        virtual void Flush() = 0;
    };

    class ReaderDecorator : public Reader {
    public:
        explicit ReaderDecorator(Scope<Reader> inner)
            : m_inner(std::move(inner))
        {
        }

        virtual ~ReaderDecorator() = default;

        size_t Read(void* buffer, size_t size) override { return m_inner->Read(buffer, size); }
        void Seek(Int64 offset, SeekOrigin origin) override { return m_inner->Seek(offset, origin); }
        size_t Tell() const override { return m_inner->Tell(); }
        size_t Size() const override { return m_inner->Size(); }

    protected:
        Scope<Reader> m_inner;
    };

    class WriterDecorator : public Writer {
    public:
        explicit WriterDecorator(Scope<Writer> inner)
            : m_inner(std::move(inner))
        {
        }

        virtual ~WriterDecorator() = default;

        size_t Write(const void* buffer, size_t size) override { return m_inner->Write(buffer, size); }
        void Flush() override { return m_inner->Flush(); }

    protected:
        Scope<Writer> m_inner;
    };
}