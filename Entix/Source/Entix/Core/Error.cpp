#include "Entix/Core/Error.h"

#include <format>
#include <filesystem>

namespace Entix
{
    String Error::What() const noexcept
    {
        return std::format(
            "{}({}:{}): {}",
            std::filesystem::path(m_location.file_name()).filename().string(),
            m_location.line(),
            m_location.column(),
            GetMessage()
        );
    }
}