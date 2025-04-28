#include <net/client.h>
#include <net/message.h>


namespace core
{
	Client::Client()
	{
	}

	void Client::connectToServer(std::string const& addr, std::string const& port)
	{
		if (m_ServerHost)
		{
			if (m_ServerHost->isConnected())
			{
				// TODO: send and handle an informative error
				return;
			}

			disconnectFromServer();
		}

		asio::ip::tcp::resolver resolver(m_IOContext);
		// It has string_view so it might fail if original string is deleted (be carrefour)
		// Also, consider a way of using async_resolve if necessary
		auto endpoints = resolver.resolve(addr, port);

		m_ServerHost = std::make_unique<Host>(asio::ip::tcp::socket(m_IOContext), m_IOContext);

		m_ServerHost->connect(endpoints);
	}

	void Client::disconnectFromServer()
	{
		m_ServerHost->closeConnection();
	}

	void Client::sendMessage(Message msg)
	{

	}

	void Client::readMessage(Message msg)
	{

	}
}