#include <ui/uiutils.h>

namespace core::ui::utils
{
	void ShowChatMessageBlock(ChatMessageInfo const& msg)
	{
		// TODO:
		// set username color based on the hostID or something
		// maybe calculate it one time somewhere else
		int val = 10;
		int diff = msg.hostID / 2 * val;
		ImVec4 color = ImVec4(1.0f, 0.3f, 0.5f, 1.0f);

		ImGui::TextColored(color, msg.userName.c_str());
		ImGui::Text(msg.text.c_str());
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