#include "socketimpl.h"

namespace core
{
	bool SocketImpl::connect(std::string const& addr, std::string const& port)
	{
		return false;
	}

	bool SocketImpl::send(void* payload)
	{
		return false;
	}

	bool SocketImpl::recv(void* payload)
	{
		return false;
	}

	void SocketImpl::close()
	{
	}

	SocketImpl::SocketImpl(boost::asio::io_context& ioContext)
		: m_IOContext(&ioContext)
		, m_Socket(*m_IOContext)
	{}
}


