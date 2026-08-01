#include "Entix/Serialization/JsonArchive.h"

#include <nlohmann/json.hpp>

namespace Entix
{
    JsonArchive::JsonArchive(IO::Stream& stream_)
        : m_stream(stream_)
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        stream.WriteText("{").Unwrap();
    }

    JsonArchive::~JsonArchive()
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        stream.WriteText("}").Unwrap();
    }

    Result<void> JsonArchive::BeginArray(StringView name) noexcept
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        EX_TRY(PreprocessWriting(stream));
        EX_TRY(stream.WriteTextFmt("\"{}\":[", name));
        m_isFirstValue = true;
        m_skipNames = true;

        return {};
    }

    Result<void> JsonArchive::EndArray() noexcept
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        EX_TRY(stream.WriteText("]"));
        m_skipNames = false;

        return {};
    }

    Result<void> JsonArchive::BeginSequence(StringView name) noexcept
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        EX_TRY(PreprocessWriting(stream));
        EX_TRY(stream.WriteTextFmt("\"{}\":{{", name));
        m_isFirstValue = true;

        return {};
    }

    Result<void> JsonArchive::EndSequence() noexcept
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        EX_TRY(stream.WriteText("}"));

        return {};
    }

    Result<void> JsonArchive::Process(StringView name, Uint32& value)
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        if(IsReading())
        {

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