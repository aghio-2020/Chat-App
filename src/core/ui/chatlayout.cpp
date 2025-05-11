#include <ui/chatlayout.h>
#include <imgui.h>

#include <string>

namespace core::ui
{
	const std::size_t k_MaxInputSize = 256;

	ChatLayout::ChatLayout(events::EventRelay<events::GUIEvents>& eventRelay, std::vector<utils::ChatMessageInfo>& messages)
		: m_EventRelay(eventRelay)
		, m_ChatMessages(messages)
	{
		m_MessageInput.reserve(k_MaxInputSize);
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
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_AlwaysOverwrite))
		{
			onMessagePosted();
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
		msgIn.text = m_MessageInput.c_str();
		msgIn.sentTime = std::chrono::system_clock::now();
		msgIn.mine = true;
		m_EventRelay.getRelay().getUserData(msgIn.username, msgIn.hostID);

		m_MessageInput.clear();

		m_ChatMessages.emplace_back(msgIn);
		m_EventRelay.getRelay().onMessagePostedToChat(msgIn);
	}
}