#pragma once

#include <common/icomponent.h>
#include <communication/broadcaster.h>
#include <net/server.h>

class ServerComponent : public core::IComponent
{
public:
	ServerComponent(core::events::Broadcaster& braodcaster);

	void init() override;
	void update() override;

private:
	void handleMessageReceived(core::messages::Message& msg, const uint32_t senderHostID);
	void handleChatMessageReceivedFrom(uint32_t senderHostID, core::messages::Message& msg);
	void handleConnectionEstablishedBy(const uint32_t senderHostID, core::messages::Message& msg);

	core::Server m_Server;
};
