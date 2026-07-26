#include <Entix/Core/EntryPoint.h>
#include <Entix/Core/Result.h>
#include <Entix/Core/String.h>
#include <Entix/Core/Memory.h>

#include <Entix/Core/Bytes.h>

#include <Entix/Core/Debug/Logger.h>
#include <Entix/Core/Debug/StreamLogSink.h>

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
        auto file = CreateScope<IO::FileStream>("log.txt", IO::StreamMode::Write);
        Logger::Instance().AddSink(CreateScope<StreamLogSink>(std::move(file)), {
            LogLevel::Trace,
            LogLevel::Debug,
            LogLevel::Info,
            LogLevel::Warning,
            LogLevel::Error,
            LogLevel::Critical
        });
        EX_LOG(LogTemp, Info, "Hello, {}!", "World");
        auto file2 = CreateScope<IO::FileStream>("test.txt", IO::StreamMode::Read);
        String msg;
        msg.resize(file2->Size());
        file2->Read(IO::ByteSpan(reinterpret_cast<std::byte*>(msg.data()), file2->Size()));
        return 0;
    }
}