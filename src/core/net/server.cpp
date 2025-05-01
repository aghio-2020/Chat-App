#include <net/server.h>

#include <iostream>

const auto k_DefaultLocalAddress = "127.0.0.1";

namespace core
{
	void log(std::string const& text)
	{
		std::cout << text << std::endl;
	}

	Server::Server()
		: m_IOContext()
	{
	}

	void Server::run(uint16_t port)
	{
		m_Acceptor = std::make_unique<asio::ip::tcp::acceptor>(m_IOContext, asio::ip::tcp::endpoint(asio::ip::make_address(k_DefaultLocalAddress), port));
		listenForConnection();
		// start ctx thread after we create a async listen loop
		// so that the ctx always has something to do and doesnt die
		m_CtxThread = std::thread([this]() { m_IOContext.run(); });
	}

	void Server::shutdown()
	{
		for (auto& host : m_Hosts)
		{
			// COULD: send a shutdown message before closing connections
			host.second->closeConnection();
		}
		m_Hosts.clear();
	}

	void Server::listenForConnection()
	{
		m_Acceptor->async_accept(
			[this](boost::system::error_code errorCode, asio::ip::tcp::socket socket) {
				if (errorCode)
				{
					// TODO: handle in callback or function
					log(errorCode.message());
					return;
				}

				onConnectionAccepted(std::move(socket));

				listenForConnection();
			});
	}

	void Server::sendMessageToClient(messages::Message const& msg, std::unique_ptr<Host>& host)
	{
		if (host->isConnected())
		{
			host->sendMessage(msg);
		}
		else
		{
			// COULD: reattempt connect
			removeHost(host);
		}
	}

	void Server::broadcastMessageToClients(messages::Message const& msg)
	{
		for (auto& host : m_Hosts)
		{
			sendMessageToClient(msg, host.second);
		}
	}

	void Server::removeHost(std::unique_ptr<Host>& host)
	{

	}

	void Server::onConnectionAccepted(asio::ip::tcp::socket&& socket)
	{
		log("host connected");

		static ID s_IDCount = 0;
		std::unique_ptr<Host> host = std::make_unique<Host>(std::move(socket), m_IOContext);
		host->onConnectionAccepted();
		m_Hosts[s_IDCount++] = std::move(host);
	}
}