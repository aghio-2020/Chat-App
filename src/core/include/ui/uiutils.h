#pragma once

#include <string>
#include <chrono>

namespace core::ui::utils
{
	struct ChatMessageInfo
	{
		std::string text;
		std::string userName;
		uint32_t hostID; // hostID returned from server
		std::chrono::system_clock::time_point sentTime = std::chrono::system_clock::now(); // TODO: get this from the message instead
		bool mine = false;
	};

	void showChatMessageBlock(ChatMessageInfo const& msg);
	bool showInputBox(std::string const& label, std::string const& hint, std::string& buf);
}