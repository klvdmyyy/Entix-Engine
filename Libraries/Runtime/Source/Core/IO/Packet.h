#pragma once

#include "Core/IO/Archive.h"

namespace IO {
    class Packet {
    public:
        virtual ~Packet() = default;

        virtual void Pack(Archive& ar) const = 0;
        virtual bool Unpack(Archive& ar) = 0;

        virtual Uint16 GetMagic() const noexcept = 0;
    };
}