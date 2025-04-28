#include <boost/asio.hpp>

#include <net/host.h>

#include <string>
#include <memory>
#include <functional>


namespace core
{
	using namespace boost;

	class Client
	{
	public:
		Client();

		void connectToServer(std::string const& addr, std::string const& port);
		void disconnectFromServer();
		void sendMessage(Message msg);
		void readMessage(Message msg);

	private:
		asio::io_context m_IOContext;
		std::unique_ptr<Host> m_ServerHost;
	};
}