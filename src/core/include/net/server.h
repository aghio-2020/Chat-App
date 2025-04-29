#include <boost/asio.hpp>

#include <net/host.h>
#include <net/message.h>

#include <string>
#include <vector>
#include <thread>
#include <memory>


namespace core
{
	using namespace boost;

	class Server
	{
	public:
		Server(uint16_t port);

		void run();
		void shutdown();

		void listenForConnection();
		void sendMessageToClient(messages::Message const& msg, std::unique_ptr<Host>& host);
		void broadcastMessageToClients(messages::Message const& msg);

	private:
		// TODO: add some sort of id to iterate over hosts
		void disconnectHost(std::unique_ptr<Host>& host);

		asio::ip::tcp::acceptor m_Acceptor;
		asio::io_context m_IOContext;
		std::thread m_CtxThread;

		std::vector<std::unique_ptr<Host>> m_Hosts;
	};
}