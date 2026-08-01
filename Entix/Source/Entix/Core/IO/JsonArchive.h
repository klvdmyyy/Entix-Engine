#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"

#include "Entix/Core/IO/Stream.h"

#include "Entix/Core/IO/Serializable.h"

#include "Entix/Core/String.h"

namespace std
{
    template<typename, typename>
    struct vector;

    template<typename, size_t>
    struct array;
}

namespace Entix
{
    class JsonArchive
    {
    public:
        explicit JsonArchive(IO::Stream& stream);
        ~JsonArchive();

        template<IO::Serializable<JsonArchive> T>
        JsonArchive& operator&(T& value)
        {
            this->Process(value);
            return *this;
        }

        ENTIX_API void Process(Uint32& value);
        ENTIX_API void Process(Int32& value);

        ENTIX_API void Process(String& value);

        ENTIX_API void BeginArray() noexcept;
        ENTIX_API void EndArray() noexcept;

        template<IO::Serializable<JsonArchive> T>
        void Process(std::vector<T>& value)
        {
            this->BeginArray();
            for(auto& elem : value)
            {
                this->Process(elem);
            }
            this->EndArray();
        }

        template<IO::Serializable<JsonArchive> T, Usize S>
        void Process(std::array<T, S>& value)
        {
            this->BeginArray();
            for(auto& elem : value)
            {
                this->Process(elem);
            }
            this->EndArray();
        }

        template<typename T>
            requires ((IO::HasSerializeMethod<T, JsonArchive>
                    || IO::HasDeserializeStaticMethod<T, JsonArchive>)
                    && std::default_initializable<T>)
        void Process(T& value)
        {
            if(this->IsReading())
            {
                if constexpr(IO::HasDeserializeStaticMethod<T, JsonArchive>)
                {
                    EX_LOG(Serialization, Warning, "Can't read value.");
                }
                else
                {
                    value = T::Deserialize(*this);
                }
            }
            else
            {
                if constexpr(IO::HasSerializeMethod<T, JsonArchive>)
                {
                    EX_LOG(Serialization, Warning, "Can't write value.");
                }
                else
                {
                    value.Serialize(*this);
                }
            }
        }

    protected:
        EX_FORCE_INLINE
        inline bool IsReading() const noexcept
        {
            return (m_stream.GetStreamMode() == IO::StreamMode::Read) ? true : false;
        }

    private:
        bool m_isArray = false;
        bool m_isFirstArrayValue = false;

        IO::Stream& m_stream;
    };
}