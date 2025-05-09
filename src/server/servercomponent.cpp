#include "servercomponent.h"

ServerComponent::ServerComponent(core::events::Broadcaster& broadcaster)
	: m_Server()
{
	m_Broadcaster = &broadcaster;
}

void ServerComponent::init()
{
	m_Server.getRelay().onHostConnection.subscribe(
		[this](uint32_t hostID)
		{
			// send the host id to the respective client
			core::messages::Message msg;
			msg.header.id = core::messages::MessageID::NOTIFICATION;
			core::messages::ConnectionEstablished msgData;
			msgData.hostID = hostID;
			msgData.serializeInto(msg);

			m_Server.sendMessageToClient(msg, hostID);
		});

	m_Server.getRelay().onMessageReceived.subscribe(
		[this](uint32_t senderHostID, core::messages::Message& msg)
		{
			m_Server.broadcastMessageToClientsExcept(senderHostID, msg);
		});

	m_Server.run(6969);
}

void ServerComponent::update()
{
}
