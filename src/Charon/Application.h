#ifndef CHARON_APP_H
#define CHARON_APP_H

#include "TCPServer.h"
#include "NscldaqSource.h"

namespace Charon {

    class Application
    {
    public:
        Application(uint16_t port, const std::string& ringName);
        ~Application();

        void Run();
    private:
        TCPServer m_server;
        NscldaqSource m_daqSource;
    };
}

#endif
