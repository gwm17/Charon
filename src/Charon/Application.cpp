#include "Application.h"

#include <iostream>

namespace Charon {

    Application::Application(uint16_t port) :
        m_server(port)
    {
    }

    Application::~Application()
    {
    }

    void Application::StartServer()
    {
        m_server.StartServer();
    }

    void Application::AttachRingBuffer()
    {

    }

    void Application::Run()
    {
        std::cout << "The ferryman is working... To stop Charon use Ctrl+C" << std::endl;
        StygianMessage tempMessage; //temporary
        while(true)
        {
            m_server.MessageClients(tempMessage);
        }
    }
}