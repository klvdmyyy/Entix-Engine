#pragma once

namespace Entix
{
    class GameClock
    {
    public:
        using TickType = unsigned long long;

        GameClock() = default;

        inline void Tick() noexcept { m_ticks++; }
        inline TickType GetTicks() const noexcept { return m_ticks; }

    private:
        TickType m_ticks = 0;
    };
}