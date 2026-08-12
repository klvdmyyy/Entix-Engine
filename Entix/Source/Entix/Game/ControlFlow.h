/**
 * @file
 * @brief Control Flow for Application class
 */
#pragma once

namespace Entix
{
    /**
     * @brief Manage your application state by this class
     */
    class ControlFlow
    {
    public:
        ControlFlow() = default;
        ~ControlFlow() = default;

        // Not copyable
        ControlFlow(const ControlFlow&) = delete;
        ControlFlow& operator=(const ControlFlow&) = delete;

        // Not movable
        ControlFlow(ControlFlow&&) = delete;
        ControlFlow& operator=(ControlFlow&&) = delete;

        inline void Quit() noexcept { m_quitRequested = true; }
        inline bool IsQuitRequested() const noexcept { return m_quitRequested; }

    private:
        bool m_quitRequested = false;
    };
}
