#include <ui/chatlayout.h>
#include <imgui.h>

#include <string>

namespace core::ui
{
	const std::size_t k_MaxInputSize = 256;

	ChatLayout::ChatLayout(events::EventRelay<events::GUIEvents>& eventRelay)
		: m_EventRelay(eventRelay)
	{
		m_MessageInput.reserve(k_MaxInputSize);
	}

	void ChatLayout::update()
	{
		ImGui::BeginChild("Chat");

		ImGui::BeginChild("MessagesView", 
			ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()), 
			ImGuiChildFlags_None, 
			ImGuiWindowFlags_HorizontalScrollbar);

		for (auto const& msg : m_ChatMessages)
		{
			utils::ShowChatMessageBlock(msg);
		}

		ImGui::EndChild();

		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing());
		if (utils::ShowInputBox("##messagein", "Write something", m_MessageInput, 
			ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_AlwaysOverwrite))
		{
			onMessagePosted();
		}

		ImGui::EndChild();
	}

	void ChatLayout::addMessageToUI(utils::ChatMessageInfo const& msg)
	{
		m_ChatMessages.emplace_back(msg);
	}

	void ChatLayout::onMessagePosted()
	{
		utils::ChatMessageInfo msgIn{ m_MessageInput.c_str(),
				"TODO: username", 0,
				std::chrono::system_clock::now(),
				true 
		};

		m_MessageInput.clear();

		addMessageToUI(msgIn);
		m_EventRelay.getRelay().onMessagePostedToChat(msgIn);
	}
}