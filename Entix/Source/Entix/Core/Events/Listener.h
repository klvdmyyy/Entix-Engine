#pragma once

namespace Entix
{
    template<typename Derived>
    class EventListener
    {
    public:
        void Subscribe()
        {
        }

        void Unsubscribe()
        {
        }

    protected:
        [[nodiscard]]
        Derived* GetDerived()
        {
            return static_cast<Derived*>(this);
        }
    };
}