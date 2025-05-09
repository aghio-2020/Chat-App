#include <ui/chatlayout.h>
#include <imgui.h>

#include <string>

namespace core::ui
{
	const std::string k_InputHint = "Enter a Message: ";
	const std::size_t k_MaxInputSize = 256;

	ChatLayout::ChatLayout(events::EventRelay<events::GUIEvents>& eventRelay)
		: m_EventRelay(eventRelay)
	{
		m_MessageInput.reserve(k_MaxInputSize);
	}

	void ChatLayout::update()
	{
		ImGui::BeginChild("Chat");

		auto space = ImGui::GetContentRegionAvail();
		// section for showing messages
		ImGui::BeginChild("Messages", ImVec2(space.x, space.y - ImGui::GetFrameHeightWithSpacing()), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar);
		for (auto const& msg : m_ChatMessages)
		{
			utils::showChatMessageBlock(msg);
		}
		ImGui::EndChild();

		if (utils::showInputBox("\t", k_InputHint, m_MessageInput))
		{
			utils::ChatMessageInfo msgIn{ m_MessageInput.c_str(),
				"TODO: username", 0, 
				std::chrono::system_clock::now(), 
				true };
			m_MessageInput.clear();

			addMessageToUI(msgIn);
			m_EventRelay.getRelay().onMessagePostedToChat(msgIn);
		}

		ImGui::EndChild();
	}

	void ChatLayout::addMessageToUI(utils::ChatMessageInfo const& msg)
	{
		m_ChatMessages.emplace_back(msg);
	}
}