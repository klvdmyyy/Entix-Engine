#pragma once

#include "Entix/Core/Base.h"
#include "Entix/Core/Globals.h"
#include "Entix/Core/Debug/Logger.h"

#include "Entix/Core/IO/Stream.h"
#include "Entix/Core/IO/TextStream.h"

#include "Entix/Serialization/Concepts.h"

#include "Entix/Core/String.h"

#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>

namespace Entix
{
    template<typename T>
    class AField
    {
    public:
        AField(StringView name_, T& value_) : name(name_), value(value_) {}

        StringView name;
        T& value;
    };

    class JsonArchive
    {
    public:
        explicit JsonArchive(IO::Stream& stream);
        ~JsonArchive();

        template<typename T>
        JsonArchive& operator&(AField<T> field)
        {
            if(auto res = this->Process(field.name, field.value); res.IsError())
            {
                EX_LOG(Serialization, Error, "Failed to serialize field '{}'", field.name);
            }
            return *this;
        }

        ENTIX_API Result<void> Process(StringView name, Uint32& value);
        ENTIX_API Result<void> Process(StringView name, Int32& value);

        ENTIX_API Result<void> Process(StringView name, String& value);

        ENTIX_API Result<void> BeginArray(StringView name) noexcept;
        ENTIX_API Result<void> EndArray() noexcept;

        ENTIX_API Result<void> BeginSequence(StringView name) noexcept;
        ENTIX_API Result<void> EndSequence() noexcept;

        template<Serializable<JsonArchive> T>
        Result<void> Process(StringView name, std::vector<T>& value)
        {
            EX_TRY(this->BeginArray(name));
            for(auto& elem : value)
            {
                EX_TRY(this->Process(name, elem));
            }
            EX_TRY(this->EndArray());
            
            return {};
        }

        template<Serializable<JsonArchive> T, Usize S>
        Result<void> Process(StringView name, std::array<T, S>& value)
        {
            EX_TRY(this->BeginArray(name));
            for(auto& elem : value)
            {
                EX_TRY(this->Process(name, elem));
            }
            EX_TRY(this->EndArray());

            return {};
        }

        template<Serializable<JsonArchive> T>
        Result<void> Process(StringView name, std::unordered_map<String, T>& value)
        {
            EX_TRY(this->BeginSequence(name));
            for(auto [name, value] : value)
            {
                EX_TRY(this->Process(name, value));
            }
            EX_TRY(this->EndSequence());

            return {};
        }

        template<typename T>
            requires ((HasSerializeMethod<T, JsonArchive>
                    || HasDeserializeStaticMethod<T, JsonArchive>)
                    && std::default_initializable<T>)
        Result<void> Process(StringView name, T& value)
        {
            if(this->IsReading())
            {
                if constexpr(HasDeserializeStaticMethod<T, JsonArchive>)
                {
                    EX_TRY(this->BeginSequence(name));
                    value = T::Deserialize(*this);
                    EX_TRY(this->EndSequence());
                }
                else
                {
                    EX_LOG(Serialization, Warning, "Can't read value.");
                }
            }
            else
            {
                if constexpr(HasSerializeMethod<T, JsonArchive>)
                {
                    EX_TRY(this->BeginSequence(name));
                    value.Serialize(*this);
                    EX_TRY(this->EndSequence());
                }
                else
                {
                    EX_LOG(Serialization, Warning, "Can't write value.");
                }
            }

            return {};
        }

    protected:
        EX_FORCE_INLINE
        inline bool IsReading() const noexcept
        {
            return (m_stream.GetStreamMode() == IO::StreamMode::Read) ? true : false;
        }

    private:
        Result<void> PreprocessWriting(IO::TextStream& stream);

        bool m_skipNames = false;
        bool m_isFirstValue = true;

        IO::Stream& m_stream;
    };
}