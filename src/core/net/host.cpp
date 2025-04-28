#include <net/host.h>

#include <type_traits>
#include <iostream>

namespace core
{
using namespace boost;

Host::Host(asio::ip::tcp::socket&& socket, asio::io_context& ioContext)
	: m_Socket(std::move(socket))
{
	m_IOContext = &ioContext;
}

Host::~Host()
{
	closeConnection();

	m_IOContext = nullptr;
}

void Host::connect(asio::ip::tcp::resolver::results_type const& endpoints)
{
	asio::async_connect(m_Socket, endpoints,
		[this](boost::system::error_code errorCode, asio::ip::tcp::endpoint)
		{
			if (errorCode)
			{
				return;
			}
			
			// start waiting for messages async
			readMessage();
		});
}

void Host::sendMessage(Message msg)
{
	m_SentMsg = msg;
	char* payload = (char*)malloc(msg.size);
	msg.serialize(payload);

	m_Socket.async_write_some(asio::buffer((void*)payload, msg.size),
		[this](boost::system::error_code const& errorCode, size_t bytesSent) {
			if (bytesSent < m_SentMsg.size)
			{
				char* missing = (char*)m_SentMsg.data + bytesSent;
				sendMessage(Message{(void*)missing, m_SentMsg.size - (uint32_t)bytesSent});
				return;
			}

			// TODO: handle message sent here
		});
}

void Host::readMessage()
{
	m_Socket.async_read_some(asio::buffer(m_ReadMsg.data, m_ReadMsg.size),
		[this](boost::system::error_code const& errorCode, size_t bytesRead)
		{

		});
}

void Host::closeConnection()
{
	m_Socket.close();
}

bool Host::isConnected() const
{
	return false;
}

}
