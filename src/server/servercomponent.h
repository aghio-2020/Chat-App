#pragma once

#include <common/icomponent.h>
#include <communication/broadcaster.h>
#include <net/server.h>

#include <map>

// if I need to send some to another host
struct HostData
{
	std::string username;
	std::string address;
	uint32_t hostID;
};

class ServerComponent : public core::IComponent
{
public:
	ServerComponent(core::events::Broadcaster& braodcaster, const char* bindingAddress, const uint16_t port);

	void init() override;
	void update() override;

private:
	void handleMessageReceived(core::messages::Message& msg, const uint32_t senderHostID);
	void handleChatMessageReceivedFrom(uint32_t senderHostID, core::messages::Message& msg);
	void handleConnectionEstablishedBy(const uint32_t senderHostID, core::messages::Message& msg);

	core::Server m_Server;

	std::vector<HostData> m_HostsData;
};
