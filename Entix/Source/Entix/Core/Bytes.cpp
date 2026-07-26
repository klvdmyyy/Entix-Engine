#include "Entix/Core/Bytes.h"

namespace Entix
{
    String Bytes::ToString() const noexcept
    {
        if(m_count >= 1024 * 1024 * 1024) {
            return std::format("{:.2f}GB", AsGigabytes());
        } else if(m_count >= 1024 * 1024) {
            return std::format("{:.2f}MB", AsMegabytes());
        } else if (m_count >= 1024) {
            return std::format("{:.2f}KB", AsKilobytes());
        } else {
            return std::format("{}B", AsBytes());
        }
    }
}