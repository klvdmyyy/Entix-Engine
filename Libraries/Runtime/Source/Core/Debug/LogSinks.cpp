#include "Core/Debug/LogSinks.h"

BufferLogSink& BufferLogSink::Instance()
{
    static BufferLogSink s_instance;
    return s_instance;
}