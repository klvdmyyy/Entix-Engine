#include "Entix/Serialization/JsonArchive.h"

#include <nlohmann/json.hpp>

namespace Entix
{
    JsonArchive::JsonArchive(IO::Stream& stream_)
        : m_stream(stream_)
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        if(IsReading())
        {
            if(auto res = stream.ReadAll(); res.IsSuccess())
                m_dataStack.emplace(json::parse(res.Unwrap()));
        }
        else
        {
            stream.WriteText("{").Unwrap();
        }
    }

    JsonArchive::~JsonArchive()
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        if(IsReading())
        {
        }
        else
        {
            stream.WriteText("}").Unwrap();
        }
    }

    Result<void> JsonArchive::BeginArray(StringView name) noexcept
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        if(IsReading())
        {
            if(!m_dataStack.top().contains(name))
                return Error(std::format("No required field with name: {}", name));

            m_dataStack.emplace(m_dataStack.top()[name]);

            if(!m_dataStack.top().is_array())
                return Error("Failed to read JSON array data!");
        }
        else
        {
            EX_TRY(PreprocessWriting(stream));
            EX_TRY(stream.WriteTextFmt("\"{}\":[", name));
            m_isFirstValue = true;
            m_skipNames = true;
        }

        return {};
    }

    Result<void> JsonArchive::EndArray() noexcept
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        if(IsReading())
        {
            m_dataStack.pop();
        }
        else
        {
            EX_TRY(stream.WriteText("]"));
            m_skipNames = false;
        }

        return {};
    }

    Result<void> JsonArchive::BeginSequence(StringView name) noexcept
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        if(IsReading())
        {
            if(!m_dataStack.top().contains(name))
                return Error(std::format("No required field with name: {}", name));

            m_dataStack.emplace(m_dataStack.top()[name]);

            if(!m_dataStack.top().is_structured())
                return Error("Failed to read JSON sequence data!");
        }
        else
        {
            EX_TRY(PreprocessWriting(stream));
            EX_TRY(stream.WriteTextFmt("\"{}\":{{", name));
            m_isFirstValue = true;
        }

        return {};
    }

    Result<void> JsonArchive::EndSequence() noexcept
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        if(IsReading())
        {
            m_dataStack.pop();
        }
        else
        {
            EX_TRY(stream.WriteText("}"));
        }

        return {};
    }

    Result<void> JsonArchive::Process(StringView name, Uint32& value)
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        if(IsReading())
        {
            if(!m_dataStack.top().contains(name))
                return Error(std::format("No required field with name: {}", name));

            auto jsonValue = m_dataStack.top()[name];

            if(!jsonValue.is_number_unsigned())
                return Error("Failed to read JSON unsgigned integer data!");

            value = jsonValue.get<Uint32>();
        }
        else
        {
            EX_TRY(PreprocessWriting(stream));
            if(!m_skipNames)
            {
                EX_TRY(stream.WriteTextFmt("\"{}\":{}", name, value));
            }
            else
            {
                EX_TRY(stream.WriteTextFmt("{}", value));
            }
        }

        return {};
    }

    Result<void> JsonArchive::Process(StringView name, Int32& value)
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        if(IsReading())
        {
            if(!m_dataStack.top().contains(name))
                return Error(std::format("No required field with name: {}", name));

            auto jsonValue = m_dataStack.top()[name];

            if(!jsonValue.is_number_integer())
                return Error("Failed to read JSON integer data!");

            value = jsonValue.get<Int32>();
        }
        else
        {
            EX_TRY(PreprocessWriting(stream));
            if(!m_skipNames)
            {
                EX_TRY(stream.WriteTextFmt("\"{}\":{}", name, value));
            }
            else
            {
                EX_TRY(stream.WriteTextFmt("{}", value));
            }
        }

        return {};
    }

    Result<void> JsonArchive::Process(StringView name, String& value)
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        if(IsReading())
        {
            if(!m_dataStack.top().contains(name))
                return Error(std::format("No required field with name: {}", name));

            auto jsonValue = m_dataStack.top()[name];

            if(!jsonValue.is_string())
                return Error("Failed to read JSON string data!");

            value = jsonValue.get<String>();
        }
        else
        {
            EX_TRY(PreprocessWriting(stream));
            if(!m_skipNames)
            {
                EX_TRY(stream.WriteTextFmt("\"{}\":\"{}\"", name, value));
            }
            else
            {
                EX_TRY(stream.WriteTextFmt("\"{}\"", value));
            }
        }

        return {};
    }

    Result<void> JsonArchive::PreprocessWriting(IO::TextStream& stream)
    {
        if(!m_isFirstValue)
        {
            EX_TRY(stream.WriteText(","));
        }
        else
        {
            m_isFirstValue = false;
        }

        return {};
    }
}