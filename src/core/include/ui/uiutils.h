#pragma once

#include <imgui.h>

#include <string>
#include <chrono>

namespace core::ui::utils
{
	struct ChatMessageInfo
	{
		std::string text;
		std::string username;
		uint32_t hostID; // hostID returned from server
		std::chrono::system_clock::time_point sentTime = std::chrono::system_clock::now(); // TODO: get this from the message instead
		bool mine = false;
	};

	struct HostInfo
	{
		std::string username;
		uint32_t hostID;
	};

	void ShowChatMessageBlock(ChatMessageInfo const& msg);
	void ShowParticipantBlock(HostInfo const& host);
	bool ShowInputBox(std::string const& label, 
		std::string const& hint, 
		std::string& buf,
		ImGuiInputTextFlags flags);
}