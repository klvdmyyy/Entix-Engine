#include "Core/Debug/Log.h"

#include "Core/Assert.h"

size_t HumanReadableLogWriter::Write(const void* data, size_t size)
{
    IO::MemoryReader reader(data, size);

    uint16_t magic;
    if(reader.Read(&magic, sizeof(magic)) != sizeof(magic) || magic != LogEntry::MAGIC_NUMBER)
    {
        EX_ASSERT_EXPR(false);
        return 0;
    }

    reader.Seek(-static_cast<Int64>(sizeof(magic)), IO::SeekOrigin::Current);

    LogEntry entry;
    EX_ASSERT_EXPR(entry.Unpack(reader));

    IO::TextWriter::CreateNonOwned(*m_inner).WriteFmt(
        "[{}] {}: {}",
        entry.level,
        entry.category.GetName(),
        entry.message
    );
    return size;
}

Logger& Logger::Instance()
{
    static Logger s_logger;
    return s_logger;
}

void Logger::AddWriter(Scope<IO::Writer> writer, std::initializer_list<LogLevel> enabledLevels) {
    LevelBitset levels;
    std::for_each(enabledLevels.begin(), enabledLevels.end(), [&](LogLevel level) { levels.set((uint8_t)level); });

    m_writers.push_back({std::move(writer), levels});
}

void Logger::WriteMessage(
    LogCategory category,
    LogLevel level,
    StringView message,
    std::source_location location
) {
    
    for (auto& [writer, enabledLevels] : m_writers) {
        if(!enabledLevels[(uint8_t)level] || !category.IsEnabled(level)) continue;

        LogEntry entry{level, category, String(message)};

        IO::PacketWriter::CreateNonOwned(*writer).Write(entry);
    }
}