#pragma once

#include "Entix/Core/Types.h"
#include "Entix/Core/Result.h"

#include <cstddef>
#include <span>

namespace Entix::IO
{
    using Byte = std::byte;
    using ByteSpan = std::span<std::byte>;
    using ConstByteSpan = std::span<const std::byte>;

    enum class StreamMode
    {
        Read = 0,
        Write,
    };

    enum class SeekOrigin
    {
        Begin = 0,
        Current,
        End,
    };

    class Stream
    {
    public:
        virtual ~Stream() = default;

        // Output
        virtual Result<void> Write(ConstByteSpan data) = 0;
        virtual void Flush() = 0;

        // Input
        virtual Result<void> Read(ByteSpan data) = 0;
        virtual Result<void> Seek(Int64 offset, SeekOrigin origin) = 0;
        virtual Usize Tell() const = 0;
        virtual Usize Size() const = 0;

        // General
        virtual StreamMode GetStreamMode() const noexcept = 0;
    };

    class StreamDecorator : public Stream
    {
    public:
        StreamDecorator(Stream* inner, bool owns = true)
            : m_inner(inner), k_owns(owns)
        {
        }

        virtual ~StreamDecorator()
        {
            if(k_owns)
            {
                delete m_inner;
            }
        }

        // Output
        virtual Result<void> Write(ConstByteSpan data) override { return m_inner->Write(data); };
        virtual void Flush() override { return m_inner->Flush(); };

        // Input
        virtual Result<void> Read(ByteSpan data) override { return m_inner->Read(data); };
        virtual Result<void> Seek(Int64 offset, SeekOrigin origin) override { return m_inner->Seek(offset, origin); };
        virtual Usize Size() const override { return m_inner->Size(); };
        virtual Usize Tell() const override { return m_inner->Tell(); }

        // General
        virtual StreamMode GetStreamMode() const noexcept override { return m_inner->GetStreamMode(); }

    protected:
        Stream* m_inner;
    
    private:
        const bool k_owns;
    };
}

template<>
struct std::formatter<::Entix::IO::StreamMode, char>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        auto it = ctx.begin();
        return it;
    }

    template<typename FmtContext>
    FmtContext::iterator format(::Entix::IO::StreamMode mode,
                                FmtContext &ctx) const
    {
        std::ostringstream out;

        switch(mode)
        {
#define ADD_MODE(X) case ::Entix::IO::StreamMode::X: out << #X; break
            ADD_MODE(Read);
            ADD_MODE(Write);
#undef ADD_MODE

            default:
                out << "Unknown";
                break;
        }

        return std::ranges::copy(std::move(out).str(), ctx.out()).out;
    }
};