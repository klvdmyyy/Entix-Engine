#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/String.h"
#include "Entix/Core/Memory.h"

#include "Entix/Core/IO/Stream.h"

#include <format>

namespace Entix::IO
{
    class TextStream : public StreamDecorator
    {
    public:
        TextStream(Scope<Stream> inner)
            : StreamDecorator(inner.release(), true)
        {
        }

        EX_FORCE_INLINE
        inline static TextStream CreateNonOwned(Stream& inner)
        {
            return TextStream(&inner, false);
        }

        Result<void> WriteText(StringView str)
        {
            return m_inner->Write(ConstByteSpan(reinterpret_cast<const std::byte*>(str.data()), str.size()));
        }

        template<typename... Args>
        Result<void> WriteTextFmt(std::format_string<Args...> fmt, Args&&... args)
        {
            return Write(std::format(fmt, std::forward<Args>(args)...));
        }

        Result<void> WriteLine(StringView str)
        {
            if(auto result = WriteText(str); result.IsError())
                return result.UnwrapErr();

            return WriteText("\n");
        }

        template<typename... Args>
        Result<void> WriteLineFmt(std::format_string<Args...> fmt, Args&&... args)
        {
            return WriteLine(std::format(fmt, std::forward<Args>(args)...));
        }

        Result<String> ReadAll()
        {
            String s;
            s.resize(m_inner->Size());

            EX_TRY(m_inner->Read(ByteSpan(reinterpret_cast<std::byte*>(s.data()), s.size())));

            return s;
        }

    private:
        TextStream(Stream* inner, bool owns)
            : StreamDecorator(inner, owns)
        {
        }
    };
}