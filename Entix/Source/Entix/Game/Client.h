#pragma once

#include "Entix/Core/Types.h"
#include "Entix/Core/String.h"

namespace Entix
{
    class Server;

    class Client
    {
    public:
        Client();
        ~Client();

        void Connect(const String& address, Uint16 port);
        void ConnectLocal(Server* server);

        void Disconnect();
    };
}