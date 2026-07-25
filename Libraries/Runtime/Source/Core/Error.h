#pragma once

#include "Core/String.h"

class Error {
public:
    constexpr Error(StringView message)
        : k_message(message)
    {
    }

    StringView What() const noexcept
    {
        return k_message;
    }

private:
    const StringView k_message;
};