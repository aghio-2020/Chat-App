#pragma once

#include <ui/chatlayout.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <ui/openglrenderer.h>
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
		WindowType windowType;
	};

	class AppGUI : public events::EventRelay<events::GUIEvents>
	{
	public:
		AppGUI(std::string const& appName);
		~AppGUI();

		void init();
		void closeApp();
		void onUpdate();
		void onNewChatMessage(core::messages::ChatMessage const& msg);

	private:
		void showBootWindowUI();
		void showMainWindowUI();

		core::ui::OpenGLRenderer m_Renderer;
		AppStateData m_State;
		ChatLayout m_ChatLayout;
	};
}