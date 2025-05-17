#include <ui/bootlayout.h>
#include <ui/uiutils.h>

#include <imgui.h>


namespace core::ui
{
	const float k_CursorOffset = 30.0f;
	const int k_StartingBufSize = 256;

	BootLayout::BootLayout(events::EventRelay<events::GUIEvents>& eventRelay)
		: m_EventRelay(eventRelay)
		, m_Username(k_StartingBufSize, '\0')
		, m_ServerAddress(k_StartingBufSize, '\0')
		, m_Port(k_StartingBufSize, '\0')
		, m_UsernameHint("Enter username")
		, m_ServerAddressHint("Enter server address")
		, m_PortHint("Enter a port")
	{
	}

	void BootLayout::update()
	{
		ImGui::BeginChild("BootLayout");

		static float inputBoxWidth = 0;
		float cursorXOffset = (ImGui::GetWindowSize().x - inputBoxWidth);

		ImGui::SetCursorPosX(cursorXOffset / 2);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + k_CursorOffset);
		if (utils::ShowInputBox("##serveraddr", m_ServerAddressHint, m_ServerAddress, 
			ImGuiInputTextFlags_EnterReturnsTrue))
		{
			onEnter();
		}
		ImGui::Spacing();
		ImGui::SetCursorPosX(cursorXOffset / 2);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + k_CursorOffset);
		if (utils::ShowInputBox("##port", m_PortHint, m_Port,
			ImGuiInputTextFlags_EnterReturnsTrue))
		{
			onEnter();
		}
		ImGui::Spacing();
		ImGui::SetCursorPosX(cursorXOffset / 2);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + k_CursorOffset);
		if (utils::ShowInputBox("##username", m_UsernameHint, m_Username, 
			ImGuiInputTextFlags_EnterReturnsTrue))
		{
			onEnter();
		}
		inputBoxWidth = ImGui::GetItemRectSize().x;


		ImGui::EndChild();
	}

	void BootLayout::setLayoutSize(float width, float height)
	{
	}

	void BootLayout::onEnter()
	{
		bool retry = false;
		if (m_ServerAddress[0] == '\0' || m_Username[0] == '\0' || m_Port[0] == '\0')
		{
			retry = true;
			m_ServerAddressHint = "Enter a valid address";
			m_UsernameHint = "Enter a valid user name";
			m_PortHint = "Enter a valid port";
		}

		if (!retry)
		{
			m_EventRelay.getRelay().onServerChosen(std::string(m_ServerAddress.data()), std::string(m_Port.data()), std::string(m_Username.data()));
		}
		else
		{
			m_Username[0] = '\0';
			m_ServerAddress[0] = '\0';
			m_Port[0] = '\0';
		}
	}
}