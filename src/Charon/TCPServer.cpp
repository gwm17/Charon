#include "TCPServer.h"

#include <iostream>

namespace Charon {

	TCPServerConnection::TCPServerConnection(asio::io_context& context, asio::ip::tcp::socket socket) :
		m_ioContextHandle(context), m_socket(std::move(socket))
	{
	}

	TCPServerConnection::~TCPServerConnection()
	{
		Disconnect();
	}

	void TCPServerConnection::Disconnect()
	{
		if (IsConnected())
			asio::post(m_ioContextHandle, [this]() { m_socket.close(); });
	}

	/*
		The job we push to asio is slightly strange. If the queue is empty when we try to push a new job to it, it tells us
		that the loop of write calls and job submissions had stopped (or was not yet started), so we need to restart/start the
		cycle of writing data to the socket. Otherwise, we just push the data onto the queue.
	*/
	void TCPServerConnection::Send(const StygianMessage& message)
	{
		asio::post(m_ioContextHandle,
			[this, message]()
			{
				bool wasEmpty = m_queue.IsEmpty();
				m_queue.PushBack(message);
				if (wasEmpty)
				{
					WriteHeader();
				}
			});
	}

	void TCPServerConnection::WriteHeader()
	{
		uint64_t messageSize = m_queue.Front().size;
		asio::async_write(m_socket, asio::buffer(&messageSize, sizeof(messageSize)),
			[this](std::error_code ec, std::size_t length) //Callback upon completion
			{
				if (!ec)
				{
					if (m_queue.Front().size > 0)
						WriteBody(); //submit next job to asio: write body data
					else
					{
						m_queue.PopFront();
						if (!m_queue.IsEmpty())
							WriteHeader(); //submit next job to asio: write the next message
					}
				}
				else
				{
					std::cout << "TCPServerConnection -> Failure to write header of message, with error code: {0}" << ec.message() << std::endl;
					std::cout << "Closing connection..." << std::endl;
					m_socket.close();
				}
			});
	}

	void TCPServerConnection::WriteBody()
	{
		asio::async_write(m_socket, asio::buffer(m_queue.Front().body, m_queue.Front().size),
			[this](std::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					m_queue.PopFront();
					if (!m_queue.IsEmpty())
						WriteHeader(); //If queue still has data, do it again.
				}
				else
				{
					std::cout << "TCPServerConnection -> Failure to write body of message, with error code: {0}" << ec.message() << std::endl;
					std::cout << "Closing connection..." << std::endl;
					m_socket.close();
				}
			});
	}
	
	TCPServer::TCPServer(uint16_t serverPort) :
		m_acceptor(m_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), serverPort)), m_acceptorPort(serverPort)
	{

	}

	TCPServer::~TCPServer()
	{
	}

	void TCPServer::StartServer()
	{
		try
		{
			WaitForClient(); //Prime the context with some work, otherwise m_context.run() will simply return!
			m_ioThread = std::thread([this]() { m_context.run(); }); //Post the context to a thread
		}
		catch (std::exception& e)
		{
			std::cerr << "Server caught exception: " << e.what() << std::endl;
			return;
		}

		std::cout << "Server has started and is listening for clients on port " << m_acceptorPort << std::endl;
	}

	void TCPServer::ShutdownServer()
	{
		m_context.stop();
		if (m_ioThread.joinable())
			m_ioThread.join();

		std::cout << "Server has been shutdown." << std::endl;
	}

	//We attempt to send a message to all clients. If we find a client who has disconnected for whatever reason,
	//we remove it from the client list and free up those resources
	void TCPServer::MessageClients(const StygianMessage& message)
	{
		bool isAnyClientInvalid = false;

		for (auto& client : m_clients)
		{
			if (client->IsConnected())
			{
				client->Send(message);
			}
			else
			{
				client->Disconnect();
				client.reset();
				isAnyClientInvalid = true;
			}
		}

		if (isAnyClientInvalid)
			m_clients.erase(std::remove(m_clients.begin(), m_clients.end(), nullptr), m_clients.end());
	}

	//Tell the acceptor to look for connections. When a connection is recieved, we add it to the client list.
	void TCPServer::WaitForClient()
	{
		m_acceptor.async_accept(
			[this](std::error_code ec, asio::ip::tcp::socket socket)
			{
				if (!ec)
				{
					std::cout << "Server connection to new client: " << socket.remote_endpoint() << std::endl;
					m_clients.push_back(std::make_shared<TCPServerConnection>(m_context, std::move(socket)));
				}
				else
					std::cerr << "Server Connection Failure with error code: " << ec.message() << std::endl;
			});
	}
}