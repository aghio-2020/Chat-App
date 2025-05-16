#include <net/server.h>
#include <net/message.h>

#include <iostream>
#include <algorithm>

const unsigned int k_MaxHostsAllowed = 256;

namespace core
{
	Server::Server(const char* bindingAddress, const uint16_t port)
	{
		m_Acceptor = std::make_unique<asio::ip::tcp::acceptor>(m_IOContext,
			asio::ip::tcp::endpoint(asio::ip::make_address(bindingAddress), port));
	}

	Server::~Server()
	{
		shutdown();
	}
	
	void Server::run()
	{
		listenForConnection();
		// start ctx thread after we create a async listen loop
		// so that the ctx always has something to do and doesnt die
		m_CtxThread = std::thread([this]() { m_IOContext.run(); });

		std::cout << "Server running!\n\n";
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
					std::cout << errorCode.message();
					return;
				}

				if (m_Hosts.size() >= k_MaxHostsAllowed)
				{
					socket.close();
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
		std::cout << "host connected: " << socket.local_endpoint().address() << "\n";

		static ID s_IDCount = 1;
		std::unique_ptr<Host> host = std::make_unique<Host>(std::move(socket), m_IOContext, static_cast<events::EventRelay<events::HostEvents>&>(*this), s_IDCount);
		m_Hosts[s_IDCount] = (std::move(host));
		m_Hosts[s_IDCount++]->onConnectionEstablished();
	}
}