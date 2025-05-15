#pragma once

#include <ui/uilayout.h>
#include <communication/eventrelay.h>
#include <communication/guievents.h>

#include <string>

namespace core::ui
{
	class BootLayout : public UILayout
	{
	public:
		BootLayout(events::EventRelay<events::GUIEvents>& eventRelay);

		void update() override;
		void setLayoutSize(float width, float height);
		
	private:
		void onEnter();

		events::EventRelay<events::GUIEvents>& m_EventRelay;
		std::vector<char> m_Username;
		std::string m_UsernameHint;
		std::vector<char> m_ServerAddress;
		std::string m_ServerAddressHint;

		float m_Width;
		float m_Height;
	};
}