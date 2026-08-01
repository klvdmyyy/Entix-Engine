#include "Entix/Core/IO/JsonArchive.h"

#include "Entix/Core/IO/TextStream.h"

#include <nlohmann/json.hpp>

namespace Entix
{
    JsonArchive::JsonArchive(IO::Stream& stream)
        : m_stream(stream)
    {
    }

    JsonArchive::~JsonArchive()
    {
    }

    void JsonArchive::Process(Uint32& value)
    {
        auto stream = IO::TextStream::CreateNonOwned(m_stream);

        if(IsReading())
        {

        }
        else
        {
            if(m_isArray)
            {
                if(!m_isFirstArrayValue)
                    stream.WriteText(",").Unwrap();
                else
                    m_isFirstArrayValue = false;

                
            }
            stream.WriteTextFmt("{}", value).Unwrap();
        }
    }
}