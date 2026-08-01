#pragma once

#include "Entix/Core/IO/Serializable.h"

namespace Entix::IO
{
    template<typename Derived>
    class Archive
    {
    public:
        template<Serializable<Derived> T>
        Derived& operator&(T& value)
        {
            // static_assert(Serializable<T, Derived>, "Type T must be Serializable with this archive type!");

            GetDerived().Process(value);
            return GetDerived();
        }

        template<typename T>
            requires (HasSerializeMethod<T, Derived>)
        void Process(T& value)
        {
            value.Serialize(GetDerived());
        }

        Derived& GetDerived() noexcept
        {
            return static_cast<Derived&>(*this);
        }

        const Derived& GetDerived() const noexcept
        {
            return static_cast<const Derived&>(*this);
        }
    };
}