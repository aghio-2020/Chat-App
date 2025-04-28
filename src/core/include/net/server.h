#include <boost/asio.hpp>

#include <net/host.h>

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
		void sendMessage();
		void broadcastMessage();

	private:
		// TODO: add some sort of id to iterate over hosts
		void disconnectHost(std::unique_ptr<Host>& host);

		asio::ip::tcp::acceptor m_Acceptor;
		asio::io_context m_IOContext;
		std::thread m_CtxThread;
		std::vector<std::unique_ptr<Host>> m_Hosts;
	};
}