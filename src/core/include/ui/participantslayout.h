#pragma once

#include <ui/uiutils.h>
#include <ui/uilayout.h>
#include <communication/eventrelay.h>
#include <communication/guievents.h>
#include <ui/uiutils.h>

#include <vector>

namespace core::ui
{
	class ParticipantsLayout : public UILayout
	{
	public:
		ParticipantsLayout(events::EventRelay<events::GUIEvents>& eventRelay, std::vector<utils::HostInfo>& participants);

		void update() override;
		void setLayoutSize(float width, float height) override;

	private:
		events::EventRelay<events::GUIEvents>& m_EventRelay;
		std::vector<utils::HostInfo>& m_Participants;

		float m_Width = 0;
		float m_Height = 0;
	};
}