#include "networkcomponent.h"
#include <net/message.h>

#include <chrono>
#include <thread>

const std::string k_DefaultServerIP = "127.0.0.1";
const std::string k_DefaultService = "6969";

NetworkComponent::NetworkComponent(core::Broadcaster& broadcaster)
{
	m_Broadcaster = &broadcaster;
}

void NetworkComponent::init()
{
	m_Client.connectToServer(k_DefaultServerIP, k_DefaultService);
}

void NetworkComponent::update()
{
	if (m_Client.isConnectedToServer())
	{
		core::messages::ExampleMsgData msgData{ 10, 100.2, "hello other world inside my local network!" };

		core::messages::Message msg;

		msg.header.category = core::messages::Category::NOTIFICATION;
		
		msgData.serializeMsgInto(msg);

		m_Client.sendMessage(msg);

		std::this_thread::sleep_for(std::chrono::seconds(2));

		m_Client.disconnectFromServer();
	}
}
