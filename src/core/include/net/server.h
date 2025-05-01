#include <boost/asio.hpp>

#include <net/host.h>
#include <net/message.h>

#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <map>


namespace core
{
	using namespace boost;

	using ID = uint32_t;

	class Server
	{
	public:
		Server();

		void run(uint16_t port);
		void shutdown();
		void broadcastMessageToClients(messages::Message const& msg);

	private:
		void listenForConnection();
		void sendMessageToClient(messages::Message const& msg, std::unique_ptr<Host>& host);
		void removeHost(std::unique_ptr<Host>& host);

		// callback functions
		void onConnectionAccepted(asio::ip::tcp::socket&& socket);

		// TODO: add some sort of id to iterate over hosts
		std::unique_ptr<asio::ip::tcp::acceptor> m_Acceptor;
		asio::io_context m_IOContext;
		std::thread m_CtxThread;

		std::map<ID, std::unique_ptr<Host>> m_Hosts;
	};
}