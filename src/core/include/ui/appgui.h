#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <ui/openglrenderer.h>
#include <ui/chatlayout.h>
#include <ui/bootlayout.h>
#include <ui/participantslayout.h>
#include <communication/eventrelay.h>
#include <communication/guievents.h>
#include <net/message.h>

#include <string>

namespace core::ui
{
	enum WindowType : uint8_t
	{
		BOOT_WINDOW,
		MAIN_WINDOW
	};

	struct AppStateData
	{
		WindowType windowType = BOOT_WINDOW;
		bool closing = false;
	};

	class AppGUI : public events::EventRelay<events::GUIEvents>
	{
	public:
		AppGUI(std::string const& appName);
		~AppGUI();

		void init();
		void closeApp();
		void onUpdate();
		AppStateData& getAppState();
		void onNewChatMessage(messages::ChatMessage const& msg);
		void onWindowChange(WindowType win);
		void onNewHostInChat(ui::utils::HostInfo const& host);

	private:
		void showBootWindowUI();
		void showMainWindowUI();
		void checkWindowSize();

		OpenGLRenderer m_Renderer;
		AppStateData m_AppState;
		ChatLayout m_ChatLayout;
		BootLayout m_BootLayout;
		ParticipantsLayout m_ParticipantsLayout;

		std::vector<ui::utils::HostInfo> m_Participants;
		std::vector<ui::utils::ChatMessageInfo> m_ChatMessages;
	};
}