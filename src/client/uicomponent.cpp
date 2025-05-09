#include "uicomponent.h"

const std::string k_AppName = "Chat App";

UIComponent::UIComponent(core::events::Broadcaster& broadcaster)
	: m_AppGui(k_AppName)
{
	m_Broadcaster = &broadcaster;
}

void UIComponent::init()
{
	m_AppGui.init();

	m_Broadcaster->subscribeToEvent(core::events::EventType::CHAT_MESSAGE_RECEIVED,
		[this](core::events::ApplicationEvent const& event)
		{
			handleChatMessageReceived(event);
		});

	m_AppGui.getRelay().onMessagePostedToChat.subscribe(
		[this](core::ui::utils::ChatMessageInfo const& msg)
		{
			// TODO: turn into CHAT_MESSAGE network message and send in CHAT_MESSAGE_POSTED event
			core::events::ChatMessagePosted msgPostEvent;
			msgPostEvent.msg.text = msg.text;
			msgPostEvent.msg.username = msg.userName;
			msgPostEvent.msg.hostID = msg.hostID;

			m_Broadcaster->pushEvent(msgPostEvent);
		});
}

void UIComponent::update()
{
	m_AppGui.onUpdate();
}

void UIComponent::handleChatMessageReceived(core::events::ApplicationEvent const& event)
{
	core::events::ChatMessageReceived const& aEvent = static_cast<core::events::ChatMessageReceived const&>(event);

	m_AppGui.onNewChatMessage(aEvent.msg);
}
