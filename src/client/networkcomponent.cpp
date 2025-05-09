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
		});

	m_Broadcaster->subscribeToEvent(core::events::EventType::SERVER_CHOSEN,
		[this](core::events::ApplicationEvent const& event)
		{
			handleServerChosen(event);
		});

	m_Client.getRelay().onHostConnection.subscribe(
		[this](uint32_t hostID)
		{
			core::events::ConnectionEstablished event;
			event.hostID = hostID;
			m_Broadcaster->pushEvent(event);
		});

	m_Client.getRelay().onHostDisconnected.subscribe(
		[this](uint32_t hostID)
		{
			core::events::HostDisconnected event;
			event.hostID = hostID;
			m_Broadcaster->pushEvent(event);
		});

	m_Client.getRelay().onMessageReceived.subscribe(
		[this](uint32_t, core::messages::Message& msg)
		{
			onMessageReceived(msg);
		});

	m_Client.getRelay().onMessageSent.subscribe(
		[this](core::messages::MessageID msgID)
		{
			core::events::MessageSent event;
			event.msgID = msgID;
			m_Broadcaster->pushEvent(event);
		});

	m_Client.connectToServer(k_DefaultServerIP, k_DefaultService);
}

void NetworkComponent::update()
{
}

void NetworkComponent::onMessageReceived(core::messages::Message& msg)
{
	// process and classify message based on category and decide what to do with it
	core::messages::MessageID msgID = msg.header.id;

	// TODO: 
	// create specific messages that have the member functions to serialize and deserialize on a raw message
	// identify using MessageID to know what message should it convert to
	switch (msgID)
	{
	case core::messages::MessageID::NOTIFICATION:
		break;
	case core::messages::MessageID::CHAT_MESSAGE:
		processReceivedChatMessage(msg);
		break;
	default:
		break;
	}
}

void NetworkComponent::processReceivedChatMessage(core::messages::Message& msg)
{
	core::events::ChatMessageReceived chatMsgEvent;

	chatMsgEvent.msg.deserializeFrom(msg);

	m_Broadcaster->pushEvent(chatMsgEvent);
}

void NetworkComponent::handleChatMessagePosted(core::events::ApplicationEvent const& event)
{
	std::cout << "message posted\n";

	core::events::ChatMessagePosted const& aEvent = static_cast<core::events::ChatMessagePosted const&>(event);

	core::messages::ChatMessage msgData;
	msgData.text = aEvent.msg.text;
	msgData.username = aEvent.msg.username;
	msgData.hostID = aEvent.msg.hostID;

	core::messages::Message msg;
	msg.header.id = core::messages::MessageID::CHAT_MESSAGE;

	msgData.serializeInto(msg);

	m_Client.sendMessage(msg);
}

void NetworkComponent::handleServerChosen(core::events::ApplicationEvent const& event)
{
	core::events::ServerChosen const& aEvent = static_cast<core::events::ServerChosen const&>(event);


	m_Client.connectToServer(k_DefaultServerIP, k_DefaultService);
}
