#include "Application.h"

#include <iostream>

namespace Charon {

    Application::Application(uint16_t port, const std::string& ringName) :
        m_server(port), m_daqSource(ringName)
    {
        m_server.StartServer();
    }

    Application::~Application()
    {
    }

    void Application::StartServer()
    {
        m_server.StartServer();
    }

    void Application::Run()
    {
        if(!m_daqSource.IsActive() || !m_server.IsActive())
        {
            std::cerr << "The ferryman cannot run when there are no souls to shuttle..." << std::endl;
            return;
        }

        std::cout << "The ferryman is working... To stop Charon use Ctrl+C" << std::endl;
        StygianMessage tempMessage; //temporary
        while(true)
        {
            tempMessage = m_daqSource.GetData();
            if(tempMessage.size == 0)
                continue;
            m_server.MessageClients(tempMessage);
        }
    }
}