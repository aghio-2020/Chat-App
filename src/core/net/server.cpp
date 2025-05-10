#include <net/server.h>
#include <net/message.h>

#include <iostream>
#include <algorithm>

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

	Server::~Server()
	{
		shutdown();
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

		if (m_CtxThread.joinable())
			m_CtxThread.join();
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
			removeHost(host->getID());
		}
	}

	void Server::broadcastMessageToClients(messages::Message const& msg)
	{
		for (auto& host : m_Hosts)
		{
			sendMessageToClient(msg, host.second);
		}
	}

	void Server::broadcastMessageToClientsExcept(const uint32_t senderHostID, messages::Message const& msg)
	{
		for (auto& host : m_Hosts)
		{
			if (host.first == senderHostID)
			{
				continue;
			}
			sendMessageToClient(msg, host.second);
		}
	}

	void Server::sendMessageToClient(messages::Message const& msg, const uint32_t hostID)
	{
		auto host = m_Hosts.find(hostID);
		if (host != m_Hosts.end())
		{
			host->second->sendMessage(msg);
		}
	}

	void Server::removeHost(const uint32_t hostID)
	{
		m_Hosts.erase(hostID);
	}

	void Server::onConnectionAccepted(asio::ip::tcp::socket&& socket)
	{
		log("host connected");

		static ID s_IDCount = 1;
		std::unique_ptr<Host> host = std::make_unique<Host>(std::move(socket), m_IOContext, static_cast<events::EventRelay<events::HostEvents>&>(*this), s_IDCount);
		m_Hosts[s_IDCount] = (std::move(host));
		m_Hosts[s_IDCount++]->onConnectionEstablished();
	}
}