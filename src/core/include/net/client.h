#include <boost/asio.hpp>

#include <net/host.h>
#include <communication/eventrelay.h>
#include <communication/hostevents.h>

#include <string>
#include <memory>
#include <functional>
#include <thread>


namespace core
{
	using namespace boost;

	class Client : public events::EventRelay<events::HostEvents>
	{
	public:
		Client();
		~Client();

		void connectToServer(std::string const& addr, std::string const& port);
		void disconnectFromServer();
		void sendMessage(messages::Message const& msg);
		bool isConnectedToServer() const;

	private:
		asio::io_context m_IOContext;
		std::unique_ptr<Host> m_ServerHost;

		std::thread m_CtxThread;
	};
}