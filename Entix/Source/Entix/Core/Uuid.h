#pragma once

#include "Entix/Core/Types.h"

namespace Entix
{
    class Uuid {
    public:
        Uuid();
        Uuid(Uint64 uuid);
        Uuid(const Uuid& other) = default;

        operator Uint64() const { return m_uuid; }

    private:
        Uint64 m_uuid;
    };
}

namespace std {
    template<typename T>
    struct hash;

    template<>
    struct hash<::Entix::Uuid>
    {
        std::size_t operator()(const ::Entix::Uuid& uuid) const {
            return (::Entix::Uint64)uuid;
        }
    };
}