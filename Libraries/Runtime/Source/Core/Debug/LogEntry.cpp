#include "Core/Debug/LogEntry.h"

#include "Core/IO/Decorators.h"

void LogEntry::Pack(IO::Writer& writer) const
{
    uint8_t header[10];
    header[0] = 0x4C;
    header[1] = 0x47;
    header[2] = 1;
    header[3] = static_cast<uint8_t>(level);

    header[4] = static_cast<uint8_t>(category.GetMinLevel());

    uint8_t categoryLength = static_cast<uint8_t>(category.GetName().size());
    header[5] = categoryLength;
    
    uint32_t msgLength = static_cast<uint32_t>(message.size());
    memcpy(&header[6], &msgLength, 4);

    writer.Write(header, sizeof(header));
    writer.Write(category.GetName().c_str(), categoryLength);
    writer.Write(message.data(), message.size());
}

bool LogEntry::Unpack(IO::Reader& reader)
{
    uint8_t header[10];
    if(reader.Read(header, sizeof(header)) != sizeof(header))
    {
        // Can't read the packet header
        return false;
    }
    
    ///////////////////////////////
    // Reading a magic number

    uint16_t magic;
    memcpy(&magic, &header[0], 2);
    if(magic != GetMagic() || header[2] != 1)
    {
        // Unknown packet or packet version
        return false;
    }

    ////////////////////////////////
    // Reading a log level

    level = static_cast<LogLevel>(header[3]);

    ////////////////////////////////
    // Reading a category min level

    auto categoryLevel = static_cast<LogLevel>(header[4]);
    
    /////////////////////////////////
    // Reading a category name

    uint8_t categoryLength = header[5];

    uint32_t msgLength;
    memcpy(&msgLength, &header[6], 4);

    String categoryName;
    categoryName.resize(categoryLength);

    if(reader.Read(categoryName.data(), categoryLength) != categoryLength)
    {
        // Can't read a category name
        return false;
    }

    category = LogCategory(categoryName, categoryLevel);

    /////////////////////////////////
    // Reading a message

    message.resize(msgLength);

    if(reader.Read(message.data(), msgLength) != msgLength)
    {
        // Can't read a message
        return false;
    }

    return true;
}