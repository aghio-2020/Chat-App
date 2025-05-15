#include <net/client.h>
#include <net/message.h>
#include <iostream>


namespace core
{
	Client::Client()
	{
	}

	Client::~Client()
	{
		if (m_CtxThread.joinable())
			m_CtxThread.join();

		disconnectFromServer();
		m_ServerHost.release();
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
			m_ServerHost.reset();
		}

		// It has string_view so it might fail if original string is deleted (be carrefour)
		// Also, consider a way of using async_resolve if necessary
		asio::ip::tcp::resolver resolver(m_IOContext);
		auto endpoints = resolver.resolve(addr, port);

		m_ServerHost = std::make_unique<Host>(asio::ip::tcp::socket(m_IOContext), m_IOContext, static_cast<events::EventRelay<events::HostEvents>&>(*this));
		m_ServerHost->connect(endpoints);

		static bool started = false;
		if (m_CtxThread.joinable())
		{
			m_CtxThread.join();
			m_CtxThread = std::thread([this]() { m_IOContext.run(); });
		}
		else if (!started)
		{
			started = true;
			m_CtxThread = std::thread([this]() { m_IOContext.run(); });
		}
	}

	void Client::disconnectFromServer()
	{
		m_ServerHost->closeConnection();
	}

	void Client::sendMessage(messages::Message const& msg)
	{
		if (isConnectedToServer())
		{
			m_ServerHost->sendMessage(msg);
		}
	}

	bool Client::isConnectedToServer() const
	{
		return m_ServerHost->isConnected();
	}

}