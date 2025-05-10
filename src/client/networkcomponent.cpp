#include "networkcomponent.h"

#include <chrono>
#include <thread>
#include <iostream>

const std::string k_DefaultServerIP = "127.0.0.1";
const std::string k_DefaultService = "6969";

NetworkComponent::NetworkComponent(core::events::Broadcaster& broadcaster)
{
	m_Broadcaster = &broadcaster;
}

void NetworkComponent::init()
{
	m_Broadcaster->subscribeToEvent(core::events::EventType::CHAT_MESSAGE_POSTED,
		[this](core::events::ApplicationEvent const& event)
		{
			handleChatMessagePosted(event);
		}
	);
	m_Broadcaster->subscribeToEvent(core::events::EventType::SERVER_CHOSEN,
		[this](core::events::ApplicationEvent const& event)
		{
			handleServerChosen(event);
		}
	);
	m_Broadcaster->subscribeToEvent(core::events::EventType::EXIT_APPLICATION,
		[this](core::events::ApplicationEvent const& event)
		{
			// TODO: handle closing connections
		}
	);

	m_Client.getRelay().onHostDisconnected.subscribe(
		[this](uint32_t hostID)
		{
			core::events::ConnectionClosed event;
			m_Broadcaster->pushEvent(event);
		}
	);
	m_Client.getRelay().onMessageSent.subscribe(
		[this](core::messages::MessageID msgID)
		{
			// TODO: 
			// handle this based on msgID
		}
	);
	m_Client.getRelay().onMessageReceived.subscribe(
		[this](uint32_t, core::messages::Message& msg)
		{
			handleMessageReceived(msg);
		}
	);
}

void NetworkComponent::update()
{
}

void NetworkComponent::onMessageSent(core::messages::MessageID msgID)
{
}

void NetworkComponent::handleMessageReceived(core::messages::Message& msg)
{
	core::messages::MessageID msgID = msg.header.id;

	switch (msgID)
	{
	case core::messages::MessageID::NOTIFICATION:
		break;

	case core::messages::MessageID::CONNECTION_ESTABLISHED: 
		std::cout << "CONNECTION_ESTABLISHED reached client, which shouldn't happen\n"; // client should send this and not receive
		break;

	case core::messages::MessageID::HOST_CONNECTION:
		handleHostConnectionMessage(msg);
		break;

	case core::messages::MessageID::HOST_DISCONNECTED:
		handleHostDisconnectionMessage(msg);
		break;

	case core::messages::MessageID::HOST_DATA:
		handleHostDataReceivedMessage(msg);
		break;

	case core::messages::MessageID::CHAT_MESSAGE:
		handleReceivedChatMessage(msg);
		break;

	default:
		break;
	}
}

void NetworkComponent::handleReceivedChatMessage(core::messages::Message& msg)
{
	core::events::ChatMessageReceived chatMsgEvent;
	chatMsgEvent.msg.deserializeFrom(msg);
	m_Broadcaster->pushEvent(chatMsgEvent);
}

void NetworkComponent::handleHostConnectionMessage(core::messages::Message& msg)
{
	core::messages::HostConnection msgPack;
	msgPack.deserializeFrom(msg);

	core::events::HostConnected hostConnEvent;
	hostConnEvent.hostID = msgPack.hostID;
	hostConnEvent.username = msgPack.username;

	m_Broadcaster->pushEvent(hostConnEvent);
}

void NetworkComponent::handleHostDisconnectionMessage(core::messages::Message& msg)
{
	// TODO
}

void NetworkComponent::handleHostDataReceivedMessage(core::messages::Message& msg)
{
	std::cout << "HOST_DATA received\n";
	core::messages::HostData msgPack;
	msgPack.deserializeFrom(msg);

	core::events::HostDataReceived dataReceivedEvent;
	dataReceivedEvent.myHostID = msgPack.hostID;
	m_HostID = msgPack.hostID;

	core::messages::Message connMsg;
	core::messages::ConnectionEstablished connMsgPack;
	connMsgPack.username = m_Username;
	connMsgPack.serializeInto(connMsg);

	m_Client.sendMessage(connMsg);
	std::cout << "sent CONNECTION_ESTABLISHED msg\n";

	m_Broadcaster->pushEvent(dataReceivedEvent);
}

void NetworkComponent::handleChatMessagePosted(core::events::ApplicationEvent const& event)
{
	core::events::ChatMessagePosted const& aEvent = static_cast<core::events::ChatMessagePosted const&>(event);

	core::messages::Message msg;
	msg.header.id = core::messages::MessageID::CHAT_MESSAGE;

	core::messages::ChatMessage msgData;
	msgData.text = aEvent.msg.text;
	msgData.username = aEvent.msg.username;
	msgData.hostID = aEvent.msg.hostID;
	msgData.serializeInto(msg);

	m_Client.sendMessage(msg);
}

void NetworkComponent::handleServerChosen(core::events::ApplicationEvent const& event)
{
	core::events::ServerChosen const& aEvent = static_cast<core::events::ServerChosen const&>(event);

	m_Username = aEvent.username;
	m_Client.connectToServer(aEvent.address, aEvent.port);
}
