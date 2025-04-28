#include <net/server.h>

#include <iostream>

const auto k_DefaultLocalAddress = "127.0.0.1";

namespace core
{
	Server::Server(uint16_t port)
		: m_IOContext()
		, m_Acceptor(m_IOContext, asio::ip::tcp::endpoint(asio::ip::make_address(k_DefaultLocalAddress), port))
	{
	}

	void Server::run()
	{
		listenForConnection();
		m_CtxThread = std::thread([this]() { m_IOContext.run(); });
	}

	void Server::shutdown()
	{

	}

	void Server::listenForConnection()
	{
		m_Acceptor.async_accept(
			[this](boost::system::error_code errorCode, asio::ip::tcp::socket socket) {
				if (errorCode)
				{
					// TODO: handle in callback or function
					std::cout << errorCode.message() << std::endl;
					return;
				}

				std::unique_ptr<Host> host = std::make_unique<Host>(std::move(socket), m_IOContext);
				m_Hosts.emplace_back(std::move(host));

				listenForConnection();
			});
	}

	void Server::sendMessage()
	{
		
	}

	void Server::broadcastMessage()
	{

	}

	void Server::disconnectHost(std::unique_ptr<Host>& host)
	{
		host->closeConnection();
	}
}