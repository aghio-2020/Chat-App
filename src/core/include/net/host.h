#pragma once

#include <boost/asio.hpp>

#include <net/message.h>
#include <common/threadsafequeue.h>

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
		void onConnectionAccepted();
		void sendMessage(messages::Message const& msg);
		void closeConnection();
		bool isConnected() const;

	private:
		void readMessageHeader();
		void readMessageBody();
		void writeMessageHeader();
		void writeMessageBody();

		asio::io_context* m_IOContext;
		asio::ip::tcp::socket m_Socket;

		utils::TSQueue<messages::Message> m_MessageWriteQueue;
		utils::TSQueue<messages::Message> m_MessageReadQueue;

		bool m_Connected = false;
	};
}