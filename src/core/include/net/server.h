#include <boost/asio.hpp>

#include <net/host.h>
#include <communication/eventrelay.h>
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

	class Server : public events::EventRelay<events::HostEvents>
	{
	public:
		Server();
		~Server();

		void run(uint16_t port);
		void shutdown();
		void broadcastMessageToClients(messages::Message const& msg);
		void broadcastMessageToClientsExcept(const uint32_t senderHostID, messages::Message const& msg);
		void sendMessageToClient(messages::Message const& msg, const uint32_t hostID);

	private:
		void listenForConnection();
		void sendMessageToClient(messages::Message const& msg, std::unique_ptr<Host>& host);
		void removeHost(const uint32_t hostID);

		// callback functions
		void onConnectionAccepted(asio::ip::tcp::socket&& socket);

		// TODO: add some sort of id to iterate over hosts
		std::unique_ptr<asio::ip::tcp::acceptor> m_Acceptor;
		asio::io_context m_IOContext;
		std::thread m_CtxThread;

		std::map<uint32_t, std::unique_ptr<Host>> m_Hosts;
	};
}