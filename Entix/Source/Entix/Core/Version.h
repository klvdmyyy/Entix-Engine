#pragma once

#include "Entix/Core/Types.h"
#include "Entix/Core/String.h"

#include "Entix/Utils/HashCombine.h"

#include <format>

namespace Entix
{
    struct Version
    {
        constexpr Version(Uint32 major_, Uint32 minor_, Uint32 patch_)
            : major(major_),
              minor(minor_),
              patch(patch_)
        {
        }

        [[nodiscard]]
        String ToString()
        {
            return std::format("{}.{}.{}", major, minor, patch);
        }

        struct Hasher
        {
            [[nodiscard]]
            Usize operator()(const Version& version) const noexcept
            {
                Usize seed = 0;
                HashCombine(seed, version.major, version.minor, version.patch);
                return seed;
            }
        };

        Uint32 major;
        Uint32 minor;
        Uint32 patch;
    };
}