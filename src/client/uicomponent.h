#pragma once

#include <common/icomponent.h>
#include <communication/broadcaster.h>
#include <string>
#include <vector>

struct ChatMsg
{
	std::string text;
};

class UIComponent : public core::IComponent
{
public:
	UIComponent(core::Broadcaster& broadcaster);

	void init() override;
	void update() override;

private:
	std::vector<ChatMsg> m_MessagesInChat;
};