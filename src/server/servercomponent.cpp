#include "servercomponent.h"

#include <iostream>

ServerComponent::ServerComponent(core::events::Broadcaster& broadcaster, const char* bindingAddress, const uint16_t port)
	: m_Server(bindingAddress, port)
{
	m_Broadcaster = &broadcaster;
}

void ServerComponent::init()
{
	m_Server.getRelay().onHostConnection.subscribe(
		[this](uint32_t hostID)
		{
			core::messages::Message cbMsg;
			core::messages::ServerData dataMsg;
			dataMsg.hostID = hostID;
			dataMsg.serializeInto(cbMsg);

			// send the host server data back to the host that just connected
			m_Server.sendMessageToClient(cbMsg, hostID);
		}
	);
	m_Server.getRelay().onHostDisconnected.subscribe(
		[this](uint32_t hostID)
		{
			core::messages::Message discMsg;
			core::messages::HostDisconnected discMsgPack;
			discMsgPack.hostID = hostID;
			discMsgPack.serializeInto(discMsg);

			m_Server.removeHost(hostID);
			auto hostData = std::find_if(m_HostsData.begin(), m_HostsData.end(), [hostID](HostData const& host) { return host.hostID == hostID; });
			if (hostData != m_HostsData.end())
			{
				m_HostsData.erase(hostData);
			}

			std::cout << "host disconnected\n";
			
			m_Server.broadcastMessageToClients(discMsg);
		}
	);
	m_Server.getRelay().onMessageReceived.subscribe(
		[this](uint32_t senderHostID, core::messages::Message& msg)
		{
			handleMessageReceived(msg, senderHostID);
		}
	);

	m_Server.run();
}

void ServerComponent::update()
{
	
}

void ServerComponent::handleMessageReceived(core::messages::Message& msg, const uint32_t senderHostID)
{
	switch (msg.header.id)
	{
	case core::messages::MessageID::NOTIFICATION:
		break;

	case core::messages::MessageID::CONNECTION_ESTABLISHED:
		handleConnectionEstablishedBy(senderHostID, msg);
		break;

	case core::messages::MessageID::HOST_CONNECTION:
		std::cout << "HOST_CONNECTION reached server. Server should only send this, not receive it\n"; // server should send this, not receive
		break;

	case core::messages::MessageID::HOST_DISCONNECTED:
		std::cout << "HOST_DISCONNECTED reached server. Server should only send this, not receive it\n"; // server should send this, not receive
		break;

	case core::messages::MessageID::SERVER_DATA:
		// COULD: 
		// let the clients update their data through this
		std::cout << "HOST_DATA reached server. Server should only send this, not receive it\n"; // server should send this, not receive
		break;

	case core::messages::MessageID::CHAT_MESSAGE:
		handleChatMessageReceivedFrom(senderHostID, msg);
		break;

	default:
		break;
	}
}

void ServerComponent::handleChatMessageReceivedFrom(uint32_t senderHostID, core::messages::Message& msg)
{
	m_Server.broadcastMessageToClientsExcept(senderHostID, msg);
}

void ServerComponent::handleConnectionEstablishedBy(const uint32_t senderHostID, core::messages::Message& msg)
{
	core::messages::ConnectionEstablished msgPack;
	msgPack.deserializeFrom(msg);

	// notify a connected host of all the other hosts in the chat
	for (auto& hostData : m_HostsData)
	{
		core::messages::Message connHostMsg;
		core::messages::HostConnection connectedHostMsgPack;
		connectedHostMsgPack.username = hostData.username;
		connectedHostMsgPack.hostID = hostData.hostID;
		connectedHostMsgPack.serializeInto(connHostMsg);
		m_Server.sendMessageToClient(connHostMsg, senderHostID);
	}

	// store a HostData with this user
	HostData senderHostData;

	// send the connected host data to other hosts already connected
	core::messages::Message relayMsg;
	core::messages::HostConnection connMsg;
	senderHostData.hostID = connMsg.hostID = senderHostID;
	senderHostData.username = connMsg.username = msgPack.username;
	connMsg.serializeInto(relayMsg);

	m_HostsData.emplace_back(senderHostData);

	// send the host data to other clients that are in the chat connection pool
	m_Server.broadcastMessageToClientsExcept(senderHostID, relayMsg);
}
