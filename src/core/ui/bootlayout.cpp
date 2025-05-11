#include <ui/bootlayout.h>
#include <ui/uiutils.h>

#include <imgui.h>


namespace core::ui
{
	const int k_MaxStrSize = 128;
	const float k_CursorOffset = 30.0f;

	BootLayout::BootLayout(events::EventRelay<events::GUIEvents>& eventRelay)
		: m_EventRelay(eventRelay)
	{
		m_Username.resize(k_MaxStrSize);
		m_ServerAddress.resize(k_MaxStrSize);

		m_UsernameHint = "Enter username";
		m_ServerAddressHint = "Enter server address";
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
			m_EventRelay.getRelay().onServerChosen(m_ServerAddress, m_Username);
		}
		else
		{

		}
	}
}