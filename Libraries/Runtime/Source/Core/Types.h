// -*- mode: c++; -*-
#pragma once

#include "Core/Base.h"

#include <cstddef>
#include <cstdint>

using Uint8 = uint8_t;
using Uint16 = uint16_t;
using Uint32 = uint32_t;
using Uint64 = uint64_t;
    
using Int8 = int8_t;
using Int16 = int16_t;
using Int32 = int32_t;
using Int64 = int64_t;

class Uuid {
public:
    Uuid();
    Uuid(Uint64 uuid);
    Uuid(const Uuid& other) = default;

    operator Uint64() const { return m_uuid; }

private:
    Uint64 m_uuid;
};

namespace std {
    template<typename T>
    struct hash;

    template<>
    struct hash<::Uuid>
    {
        std::size_t operator()(const ::Uuid& uuid) const {
            return (::Uint64)uuid;
        }
    };
}