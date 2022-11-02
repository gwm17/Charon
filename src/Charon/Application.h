#ifndef CHARON_APP_H
#define CHARON_APP_H

#include "TCPServer.h"

namespace Charon {

    class Application
    {
    public:
        Application(uint16_t port);
        ~Application();

        void StartServer();
        void AttachRingBuffer();
        void Run();

    private:
        TCPServer m_server;
    };
}

#endif