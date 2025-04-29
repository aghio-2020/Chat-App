#include <net/host.h>

#include <type_traits>
#include <iostream>

namespace core
{
using namespace boost;

// TODO: have a map with callbacks to call on certain events

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
			m_Connected = true;
			// start awaiting for messages async
			readMessageHeader();
		});
}

void Host::readMessageHeader()
{
	messages::Message msg;
	m_MessageReadQueue.push_back(msg);
	m_Socket.async_read_some(asio::buffer(&m_MessageReadQueue.front().header, sizeof messages::MessageHeader),
		[this](boost::system::error_code errorCode, std::size_t bytes)
		{
			if (!errorCode)
			{
				if (bytes < sizeof messages::MessageHeader)
				{
					// TODO: handle case
				}
				if (m_MessageReadQueue.front().header.size > 0)
				{
					readMessageBody();
				}
				else
				{
					readMessageHeader();	
				}
			}
			else
			{
				// TODO: handle errors
			}
		});
}

void Host::readMessageBody()
{
	m_Socket.async_read_some(asio::buffer(m_MessageReadQueue.front().body.data(), m_MessageReadQueue.front().header.size),
		[this](boost::system::error_code errorCode, std::size_t bytes)
		{
			if (!errorCode)
			{
				if (bytes < m_MessageReadQueue.front().header.size)
				{
					// TODO: handle case
				}
				readMessageHeader();
			}
			else
			{
				// TODO: handle errors
			}
		});
}

void Host::sendMessage(messages::Message const& msg)
{
	asio::post(*m_IOContext,
		[this, msg]()
		{
			m_MessageWriteQueue.push_back(msg);
			if (m_MessageWriteQueue.empty())
			{
				writeMessageHeader();
			}
		});
}

void Host::writeMessageHeader()
{
	// you can pass a memory location to void* with & as well
	m_Socket.async_write_some(asio::buffer(&m_MessageWriteQueue.front().header, sizeof messages::MessageHeader),
		[this](boost::system::error_code errorCode, std::size_t bytes)
		{
			if (!errorCode)
			{
				if (bytes < sizeof messages::MessageHeader)
				{
					// TODO: handle case
				}
				if (!m_MessageWriteQueue.front().body.empty())
				{
					writeMessageBody();
				}
				else
				{
					m_MessageWriteQueue.pop_front();

					if (!m_MessageWriteQueue.empty())
					{
						// start async operation to write the next msg
						writeMessageHeader();
					}
				}
			}
			else
			{
				// TODO: handle error
			}
		});
}

void Host::writeMessageBody()
{
	m_Socket.async_write_some(asio::buffer(&m_MessageWriteQueue.front().body, sizeof m_MessageWriteQueue.front().body.size()),
		[this](boost::system::error_code errorCode, std::size_t bytes)
		{
			if (bytes < m_MessageWriteQueue.front().body.size())
			{
				// TODO: handle case
			}
			if (!errorCode)
			{
				m_MessageWriteQueue.pop_front();

				if (!m_MessageWriteQueue.empty())
				{
					writeMessageHeader();
				}
			}
			else
			{
				// TODO: handle error
			}
		});
}

void Host::closeConnection()
{
	m_Socket.close();
	m_Connected = false;
}

bool Host::isConnected() const
{
	return m_Connected && m_Socket.is_open();
}

}
