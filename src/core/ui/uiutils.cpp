#include <ui/uiutils.h>
#include <imgui.h>

namespace core::ui::utils
{
	void showChatMessageBlock(ChatMessageInfo const& msg)
	{
		int val = 10;
		int diff = msg.hostID / 2 * val;
		ImVec4 color = ImVec4(1.0f, 0.3f, 0.5f, 1.0f);

		ImGui::TextColored(color, msg.userName.c_str());
		ImGui::Text(msg.text.c_str());
	}

	bool showInputBox(std::string const& label, std::string const& hint, std::string& buf)
	{
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing());

		return ImGui::InputTextWithHint(label.c_str(), hint.c_str(), buf.data(), buf.capacity(), 
			ImGuiInputTextFlags_EnterReturnsTrue);
	}
}