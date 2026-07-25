#pragma once

#include "Core/Types.h"

#include <type_traits>

namespace IO {
    class Packet;

    class Archive {
    public:
        virtual ~Archive() = default;

        virtual bool IsWriting() const noexcept = 0;
        virtual bool IsReading() const noexcept { return !IsWriting(); }

        virtual void Process(bool& val, const char* name) = 0;

        virtual void Process(Uint8& val, const char* name) = 0;
        virtual void Process(Uint16& val, const char* name) = 0;
        virtual void Process(Uint32& val, const char* name) = 0;
        virtual void Process(Uint64& val, const char* name) = 0;

        virtual void Process(String& val, const char* name) = 0;

        template<typename Enum, typename T = Uint8>
        void ProcessEnum(Enum& val, const char* name)
        {
            static_assert(std::is_enum<Enum>::value, "Non-enum type!");
            static_assert(std::is_integral<T>::value, "T must be an integer which represents Enum values!");

            Process(static_cast<T>(val), name);
        }

        virtual void BeginArray() = 0;
        virtual void EndArray() = 0;
    };
}