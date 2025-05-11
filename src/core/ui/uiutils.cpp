#include <ui/uiutils.h>

namespace core::ui::utils
{
	void ShowChatMessageBlock(ChatMessageInfo const& msg)
	{
		// TODO:
		// set username color based on the hostID or something
		// maybe calculate it one time somewhere else
		float diff = static_cast<float>(msg.hostID) / 0xFFFFFFFF;
		diff *= msg.username.size();
		ImVec4 color = ImVec4(1.0f, 0.3f, 0.4f, 1.0f);

		ImGui::TextColored(color, msg.username.c_str());
		ImGui::Text(msg.text.c_str());
	}

	void ShowParticipantBlock(HostInfo const& host)
	{
		float diff = static_cast<float>(host.hostID) / 0xFFFFFFFF;
		diff *= host.username.size();
		ImVec4 color = ImVec4(1.0f, 0.3f, 0.4f, 1.0f);

		ImGui::TextColored(color, host.username.c_str());
		ImGui::Text("connected");
	}

	bool ShowInputBox(std::string const& label, 
		std::string const& hint, 
		std::string& buf, 
		ImGuiInputTextFlags flags)
	{
		return ImGui::InputTextWithHint(label.c_str(), 
			hint.c_str(), 
			buf.data(), 
			buf.capacity(), 
			flags
		);
	}
}