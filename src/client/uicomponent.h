#pragma once

#include <common/icomponent.h>
#include <communication/broadcaster.h>
#include <ui/appgui.h>

#include <string>
#include <vector>

struct ChatMsg
{
	std::string text;
};

class UIComponent : public core::IComponent
{
public:
	UIComponent(core::events::Broadcaster& broadcaster);

	void init() override;
	void update() override;

private:
	void handleChatMessageReceived(core::events::ApplicationEvent const& event);

	core::ui::AppGUI m_AppGui;
};