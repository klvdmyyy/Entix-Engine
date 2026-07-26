#include <Entix/Core/EntryPoint.h>
#include <Entix/Core/Result.h>
#include <Entix/Core/String.h>
#include <Entix/Core/Memory.h>

#include <Entix/Core/Bytes.h>

#include <Entix/Core/Debug/Logger.h>

#include <Entix/Core/Globals.h>

#include <Entix/Core/IO/FileStream.h>
#include <Entix/Core/IO/TextStream.h>

#include <cstdlib>
#include <print>

namespace Entix
{
    class MyError : public Error
    {
    public:
        MyError(std::source_location location = std::source_location::current())
            : Error("It's my error", location)
        {
        }
    };

    int Main(int argc, char** argv)
    {
        IO::FileStream file("log.txt", IO::StreamMode::Write);
        auto res = IO::TextStream::CreateNonOwned(file).WriteLineFmt("Hello, {}!", "World");
        if(res.IsError())
            EX_LOG(LogTemp, Error, "File error: {}", res.UnwrapErr());
            
        return 0;
    }
}