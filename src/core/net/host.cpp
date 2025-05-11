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
				std::cout << errorCode.message();
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
	asio::async_read(m_Socket, asio::buffer(&m_TempMessageRead.header, sizeof(messages::MessageHeader)),
		[this](boost::system::error_code errorCode, std::size_t bytes)
		{
			if (!errorCode)
			{
				std::cout << "header read\n";
				if (bytes < sizeof(messages::MessageHeader))
				{
					// TODO: handle case
				}
				if (m_TempMessageRead.header.size > 0)
				{
					m_TempMessageRead.body.resize(m_TempMessageRead.header.size);
					readMessageBody();
				}
				else
				{
					m_EventRelay.getRelay().onMessageReceived(getID(), m_TempMessageRead);
					m_TempMessageRead.cleanup();

					// continue listening for messages
					readMessageHeader();
				}
			}
			else
			{
				std::cout << errorCode.message() << " while reading body\n";
				// TODO: handle errors
			}
		});
}

void Host::readMessageBody()
{
	asio::async_read(m_Socket, asio::buffer(m_TempMessageRead.body.data(), m_TempMessageRead.header.size),
		[this](boost::system::error_code errorCode, std::size_t bytes)
		{
			if (!errorCode)
			{
				std::cout << "body read\n";
				if (bytes < m_TempMessageRead.header.size)
				{
					// TODO: handle case
				}

				// handle message, probably relay if server, send to ui if client (defined on each app)
				m_EventRelay.getRelay().onMessageReceived(getID(), m_TempMessageRead);
				m_TempMessageRead.cleanup();

				// start listening for new messages
				readMessageHeader();
			}
			else
			{
				std::cout << errorCode.message() << " while reading body\n";
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
			if (!errorCode)
			{
				std::cout << "header written\n";
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
				std::cout << errorCode.message() << " while writing header\n";
				// TODO: handle error
			}
		});
}

void Host::writeMessageBody()
{
	m_Socket.async_write_some(asio::buffer(m_MessageWriteQueue.front().body.data(), m_MessageWriteQueue.front().body.size()),
		[this](boost::system::error_code errorCode, std::size_t bytes)
		{
			if (bytes < m_MessageWriteQueue.front().body.size())
			{
				// TODO: handle case
			}
			if (!errorCode)
			{
				std::cout << "body written\n";
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
				std::cout << errorCode.message() << " while writing body\n";
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
