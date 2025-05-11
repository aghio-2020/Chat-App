#include <ui/participantslayout.h>


namespace core::ui
{
	ParticipantsLayout::ParticipantsLayout(events::EventRelay<events::GUIEvents>& eventRelay, std::vector<utils::HostInfo>& participants)
		: m_Participants(participants)
		, m_EventRelay(eventRelay)
	{

	}

	void ParticipantsLayout::update()
	{
		ImGui::BeginChild("Participants", ImVec2(m_Width, m_Height), ImGuiChildFlags_Borders, ImGuiWindowFlags_MenuBar);

		for (auto& participant : m_Participants)
		{
			utils::ShowParticipantBlock(participant);
			ImGui::Spacing();
		}

		ImGui::EndChild();
	}
	void ParticipantsLayout::setLayoutSize(float width, float height)
	{
		m_Width = width;
		m_Height = height; 
	}
}