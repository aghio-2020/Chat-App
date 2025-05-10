#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>

#include <ui/openglrenderer.h>
#include <ui/chatlayout.h>
#include <ui/bootlayout.h>
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

	private:
		void showBootWindowUI();
		void showMainWindowUI();

		OpenGLRenderer m_Renderer;
		AppStateData m_AppState;
		ChatLayout m_ChatLayout;
		BootLayout m_BootLayout;
	};
}