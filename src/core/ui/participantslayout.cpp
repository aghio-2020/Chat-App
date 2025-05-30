#include <ui/participantslayout.h>


namespace core::ui
{
	ParticipantsLayout::ParticipantsLayout(std::vector<utils::HostInfo>& participants)
		: m_Participants(participants)
	{

	}

	void ParticipantsLayout::update()
	{
		ImGui::BeginChild("Participants", ImVec2(m_Width, m_Height), ImGuiChildFlags_Borders, ImGuiWindowFlags_ChildMenu);
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 childPos = ImGui::GetCursorPos();
		ImVec2 pos = { winPos.x + childPos.x, winPos.y + childPos.y };
		ImGui::SetCursorPos(pos);
		ImGui::Text("Participants");
		ImGui::NewLine();

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