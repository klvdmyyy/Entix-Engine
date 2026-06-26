#include "Core/Debug/LogSinks.h"

namespace ERUNTIME_NAMESPACE {
    BufferLogSink& BufferLogSink::Instance()
    {
        static BufferLogSink s_instance;
        return s_instance;
    }
}