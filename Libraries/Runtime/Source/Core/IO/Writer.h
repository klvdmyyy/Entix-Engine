// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"

#include "Core/String.h"

#include <format>

namespace IO {
    class ERUNTIME_API Writer {
    public:
        virtual ~Writer() = default;

        virtual void Write(StringView str) = 0;

        FORCE_INLINE
        inline virtual void WriteLine(StringView str)
        {
            Write(str);
            Write("\n");
        }

        template<typename... Args>
        inline void WriteFmt(std::format_string<Args...> fmt, Args&&... args)
        {
            Write(std::format(fmt, std::forward<Args>(args)...));
        }

        template<typename... Args>
        inline void WriteLineFmt(std::format_string<Args...> fmt, Args&&... args)
        {
            WriteLine(std::format(fmt, std::forward<Args>(args)...));
        }
    };
}