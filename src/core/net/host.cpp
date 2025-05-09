#include <net/host.h>
#include <boost/system/errc.hpp>

#include <type_traits>
#include <iostream>

void log(std::string const& text)
{
	std::cout << text << std::endl;
}

namespace core
{
using namespace boost;

// TODO: have a map with callbacks to call on certain events

Host::Host(asio::ip::tcp::socket&& socket, asio::io_context& ioContext, HostRelay& eventRelay, uint32_t id)
	: m_Socket(std::move(socket))
	, m_IOContext(ioContext)
	, m_EventRelay(eventRelay)
	, m_HostID(id)
{
}

Host::~Host()
{
	closeConnection();
}

void Host::connect(asio::ip::tcp::resolver::results_type const& endpoints)
{
	asio::async_connect(m_Socket, endpoints,
		[this](boost::system::error_code errorCode, asio::ip::tcp::endpoint)
		{
			std::cout << "client connected\n";
			if (errorCode)
			{
				return;
			}
			// start awaiting for messages async
			onConnectionEstablished();
		});
}

void Host::onConnectionEstablished()
{
	if (!isConnected())
	{
		m_Connected = true;
		readMessageHeader();

		m_EventRelay.getRelay().onHostConnection(m_HostID);
	}
}

uint32_t Host::getID() const
{
	return m_HostID;
}

void Host::readMessageHeader()
{
	messages::Message msg;
	m_MessageReadQueue.push_back(msg);
	m_Socket.async_read_some(asio::buffer(&m_MessageReadQueue.front().header, sizeof(messages::MessageHeader)),
		[this](boost::system::error_code errorCode, std::size_t bytes)
		{
			std::cout << "message read\n";
			if (!errorCode)
			{
				if (bytes < sizeof(messages::MessageHeader))
				{
					// TODO: handle case
				}
				if (m_MessageReadQueue.front().header.size > 0)
				{
					readMessageBody();
				}
				else
				{
					m_EventRelay.getRelay().onMessageReceived(getID(), m_MessageReadQueue.front());

					m_MessageReadQueue.pop();
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

				m_EventRelay.getRelay().onMessageReceived(getID(), m_MessageReadQueue.front());

				// start listening for new messages
				m_MessageReadQueue.pop();
				readMessageHeader();
			}
			else
			{
				std::cout << errorCode.message();
				// TODO: handle errors
			}
		});
}

void Host::sendMessage(messages::Message const& msg)
{
	asio::post(m_IOContext,
		[this, msg]()
		{
			m_MessageWriteQueue.push_back(msg);
			if (!m_Writing)
			{
				writeMessageHeader();
			}
		});
}

void Host::writeMessageHeader()
{
	m_Writing = true;
	// you can pass a memory location to void* with & as well
	m_Socket.async_write_some(asio::buffer(&m_MessageWriteQueue.front().header, sizeof(messages::MessageHeader)),
		[this](boost::system::error_code errorCode, std::size_t bytes)
		{
			std::cout << "message written\n";
			if (!errorCode)
			{
				if (bytes < sizeof(messages::MessageHeader))
				{
					// TODO: handle case
				}
				if (!m_MessageWriteQueue.front().body.empty())
				{
					writeMessageBody();
				}
				else
				{
					m_MessageWriteQueue.pop();

					if (!m_MessageWriteQueue.empty())
					{
						// start async operation to write the next msg
						writeMessageHeader();
					}
					else
					{
						m_Writing = false;
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
	m_Socket.async_write_some(asio::buffer(&m_MessageWriteQueue.front().body, m_MessageWriteQueue.front().body.size()),
		[this](boost::system::error_code errorCode, std::size_t bytes)
		{
			if (bytes < m_MessageWriteQueue.front().body.size())
			{
				// TODO: handle case
			}
			if (!errorCode)
			{
				m_MessageWriteQueue.pop();

				if (!m_MessageWriteQueue.empty())
				{
					writeMessageHeader();
				}
				else
				{
					m_Writing = false;
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
	m_Connected = false;
	m_Socket.close();
}

bool Host::isConnected() const
{
	return m_Connected && m_Socket.is_open();
}

}
