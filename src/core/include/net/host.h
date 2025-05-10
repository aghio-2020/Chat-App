#pragma once

#include <boost/asio.hpp>

#include <net/message.h>
#include <common/threadsafequeue.h>
#include <communication/eventrelay.h>
#include <communication/hostevents.h>

#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <mutex>

namespace core
{
	using namespace boost;

	using HostRelay = events::EventRelay<events::HostEvents>;

	class Host
	{
	public:
		Host(asio::ip::tcp::socket&& socket, asio::io_context& ioContext, HostRelay& eventRelay, uint32_t id = 0);
		~Host();

		void connect(asio::ip::tcp::resolver::results_type const& endpoints);
		void onConnectionEstablished();
		void sendMessage(messages::Message const& msg);
		void closeConnection();
		bool isConnected() const;
		uint32_t getID() const;

	private:
		void readMessageHeader();
		void readMessageBody();
		void writeMessageHeader();
		void writeMessageBody();

		asio::io_context& m_IOContext;
		asio::ip::tcp::socket m_Socket;

		utils::TSQueue<messages::Message> m_MessageWriteQueue;
		messages::Message m_TempMessageRead;

		uint32_t m_HostID;

		std::mutex m_Mutex;

		HostRelay& m_EventRelay;

		bool m_Connected = false;
		bool m_Writing = false;
	};
}