#pragma once

#include <common/icomponent.h>
#include <communication/broadcaster.h>
#include <ui/appgui.h>

#include <string>
#include <vector>

struct MyUserData
{
	std::string username;
	std::string currServerAddress;
	uint32_t hostID;
};

class UIComponent : public core::IComponent
{
public:
	UIComponent(core::events::Broadcaster& broadcaster);

	void init() override;
	void update() override;

private:
	void handleChatMessageReceived(core::events::ApplicationEvent const& event);
	void handleHostConnectedToChat(core::events::ApplicationEvent const& event);
	void handleHostDataReceived(core::events::ApplicationEvent const& event);

	core::ui::AppGUI m_AppGui;

	MyUserData m_UserData;
};