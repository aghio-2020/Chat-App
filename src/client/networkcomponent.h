#pragma once

#include <common/icomponent.h>
#include <communication/broadcaster.h>
#include <net/client.h>
#include <net/message.h>
#include <communication/applicationevents.h>

class NetworkComponent : 
	public core::IComponent
{
public:
	NetworkComponent(core::events::Broadcaster& broadcaster);
	
	void init() override;
	void update() override;

private:
	void onMessageReceived(core::messages::Message& msg);
	void onMessageSent(core::messages::MessageID msgID);
	void processReceivedChatMessage(core::messages::Message& msg);

private:
	void handleChatMessagePosted(core::events::ApplicationEvent const& event);
	void handleServerChosen(core::events::ApplicationEvent const& event);

	core::Client m_Client;
};