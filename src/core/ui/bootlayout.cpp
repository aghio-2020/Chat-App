#include <ui/bootlayout.h>
#include <ui/uiutils.h>

#include <imgui.h>


namespace core::ui
{
	const float k_CursorOffset = 30.0f;
	const int k_StartingBufSize = 256;

	BootLayout::BootLayout(events::EventRelay<events::GUIEvents>& eventRelay)
		: m_EventRelay(eventRelay)
		, m_Username(k_StartingBufSize)
		, m_ServerAddress(k_StartingBufSize)
		, m_UsernameHint("Enter username")
		, m_ServerAddressHint("Enter server address")
	{
	}

	void BootLayout::update()
	{
		ImGui::BeginChild("BootLayout");

		ImGui::SetCursorPosX(ImGui::GetCursorPosY() + k_CursorOffset);
		if (utils::ShowInputBox("##1", m_ServerAddressHint, m_ServerAddress, 
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_AlwaysOverwrite))
		{
			onEnter();
		}
		ImGui::Spacing();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + k_CursorOffset);
		if (utils::ShowInputBox("##2", m_UsernameHint, m_Username, 
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_AlwaysOverwrite))
		{
			onEnter();
		}

		ImGui::EndChild();
	}

	void BootLayout::setLayoutSize(float width, float height)
	{
	}

	void BootLayout::onEnter()
	{
		bool retry = false;
		if (m_ServerAddress.empty())
		{
			retry = true;
			m_ServerAddressHint = "Enter a valid address";
		}
		if (m_Username.empty())
		{
			retry = true;
			m_UsernameHint = "Enter a valid user name";
		}

		if (!retry)
		{
			m_EventRelay.getRelay().onServerChosen(std::string(m_ServerAddress.data()), std::string(m_Username.data()));
		}
		else
		{
			m_Username[0] = '\0';
			m_ServerAddress[0] = '\0';
		}
	}
}