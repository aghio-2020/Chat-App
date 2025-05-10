#include "uicomponent.h"

const std::string k_AppName = "Chat App";

UIComponent::UIComponent(core::events::Broadcaster& broadcaster)
	: m_AppGui(k_AppName)
{
	m_Broadcaster = &broadcaster;
}

void UIComponent::init()
{
	m_Broadcaster->subscribeToEvent(core::events::EventType::EXIT_APPLICATION,
		[this](core::events::ApplicationEvent const& event)
		{
			m_AppGui.closeApp();
		}
	);
	m_Broadcaster->subscribeToEvent(core::events::EventType::CHAT_MESSAGE_RECEIVED,
		[this](core::events::ApplicationEvent const& event)
		{
			handleChatMessageReceived(event);
		}
	);
	m_Broadcaster->subscribeToEvent(core::events::EventType::HOST_DATA_RECEIVED,
		[this](core::events::ApplicationEvent const& event)
		{
			handleHostDataReceived(event);
		}
	);
	m_Broadcaster->subscribeToEvent(core::events::EventType::HOST_CONNECTED,
		[this](core::events::ApplicationEvent const& event)
		{
			handleHostConnectedToChat(event);
		}
	);

	m_AppGui.getRelay().onMessagePostedToChat.subscribe(
		[this](core::ui::utils::ChatMessageInfo const& msg)
		{
			// TODO: turn into CHAT_MESSAGE network message and send in CHAT_MESSAGE_POSTED event
			core::events::ChatMessagePosted msgPostEvent;
			msgPostEvent.msg.text = msg.text;
			msgPostEvent.msg.username = msg.userName;
			msgPostEvent.msg.hostID = msg.hostID;

			m_Broadcaster->pushEvent(msgPostEvent);
		}
	);
	m_AppGui.getRelay().onServerChosen.subscribe(
		[this](std::string const& serverAddress, std::string const& username)
		{
			core::events::ServerChosen event;
			m_UserData.currServerAddress = event.address = serverAddress;
			m_UserData.username = event.username = username;
			event.port = "6969";

			m_Broadcaster->pushEvent(event);
		}
	);
	m_AppGui.getRelay().onShouldQuit.subscribe(
		[this]()
		{
			m_AppGui.getAppState().closing = true;

			core::events::ExitApplication exitEvent;
			m_Broadcaster->pushEvent(exitEvent);
		}
	);

	m_AppGui.init();
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

// an external host connected to the chat room
void UIComponent::handleHostConnectedToChat(core::events::ApplicationEvent const& event)
{
	core::events::HostConnected const& aEvent = static_cast<core::events::HostConnected const&>(event);

	
}

// server returned host data after connection likely
void UIComponent::handleHostDataReceived(core::events::ApplicationEvent const& event)
{
	core::events::HostDataReceived const& aEvent = static_cast<core::events::HostDataReceived const&>(event);

	m_UserData.hostID = aEvent.myHostID;
	m_AppGui.onWindowChange(core::ui::WindowType::MAIN_WINDOW);
}
