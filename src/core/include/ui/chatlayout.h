#pragma once

#include <ui/uilayout.h>
#include <ui/uiutils.h>
#include <communication/eventrelay.h>
#include <communication/guievents.h>

#include <string>
#include <chrono>

namespace core::ui
{
	class ChatLayout : public UILayout
	{
	public:
		ChatLayout(events::EventRelay<events::GUIEvents>& eventRelay);

		void update() override;
		void addMessageToUI(utils::ChatMessageInfo const& msg);

	private:
		void onMessagePosted();

		events::EventRelay<events::GUIEvents>& m_EventRelay;
		std::vector<utils::ChatMessageInfo> m_ChatMessages;
		std::string m_MessageInput;
	};
}