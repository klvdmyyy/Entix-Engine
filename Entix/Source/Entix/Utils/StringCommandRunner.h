#pragma once

namespace Entix
{
    class StringCommandRunner
    {
    public:
        static StringCommandRunner& Instance();

    private:
        StringCommandRunner() = default;
    };
}