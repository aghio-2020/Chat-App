#include <ui/chatlayout.h>
#include <imgui.h>

#include <string>

namespace core::ui
{
	const int k_StartingBufSize = 256;

	ChatLayout::ChatLayout(events::EventRelay<events::GUIEvents>& eventRelay, std::vector<utils::ChatMessageInfo>& messages)
		: m_EventRelay(eventRelay)
		, m_ChatMessages(messages)
		, m_MessageInput(k_StartingBufSize)
	{
	}

	void ChatLayout::update()
	{
		float paddingSize = ImGui::GetFrameHeightWithSpacing();
		// deduce border size to avoid a scrollbar appearing when not needed
		ImGui::BeginChild("Chat", ImVec2(m_Width, m_Height));

		ImGui::BeginChild("MessagesView", 
			ImVec2(m_Width, m_Height - paddingSize),
			ImGuiChildFlags_None, 
			ImGuiWindowFlags_HorizontalScrollbar);

		for (auto const& msg : m_ChatMessages)
		{
			utils::ShowChatMessageBlock(msg);
		}

		ImGui::EndChild();

		ImGui::SetCursorPosY(m_Height - paddingSize);
		if (utils::ShowInputBox("##messagein", "Write something", m_MessageInput, 
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AllowTabInput))
		{
			onMessagePosted();
			if (!ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
			{
				ImGui::SetKeyboardFocusHere(-1);
			}
		}

		ImGui::EndChild();
	}

	void ChatLayout::setLayoutSize(float width, float height)
	{
		m_Width = width;
		m_Height = height;
	}

	void ChatLayout::onMessagePosted()
	{
		utils::ChatMessageInfo msgIn;
		msgIn.text = m_MessageInput.data();
		msgIn.sentTime = std::chrono::system_clock::now();
		msgIn.mine = true;
		m_EventRelay.getRelay().getUserData(msgIn.username, msgIn.hostID);

		m_MessageInput[0] = '\0';

		m_ChatMessages.emplace_back(msgIn);
		m_EventRelay.getRelay().onMessagePostedToChat(msgIn);
	}
}