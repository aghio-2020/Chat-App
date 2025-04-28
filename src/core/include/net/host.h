#pragma once

#include <boost/asio.hpp>

#include <net/message.h>

#include <vector>
#include <memory>
#include <map>
#include <functional>

namespace core
{
	using namespace boost;

	class Host
	{
	public:
		Host(asio::ip::tcp::socket&& socket, asio::io_context& ioContext);
		~Host();

		void connect(asio::ip::tcp::resolver::results_type const& endpoints);
		void sendMessage(Message msg);
		void readMessage();
		void closeConnection();
		bool isConnected() const;

	private:
		asio::io_context* m_IOContext;
		asio::ip::tcp::socket m_Socket;

		Message m_ReadMsg;
		Message m_SentMsg;
	};
}