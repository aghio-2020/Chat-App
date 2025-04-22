#pragma once

#include <boost/asio.hpp>
#include <string>

#include <net/isocket.h>
#include <net/socketfactory.h>

namespace core
{
    class SocketImpl : public ISocket
    {
        friend class SocketFactory;

    public:
        bool connect(std::string const& host, std::string const& port) override;
        bool send(void* payload) override;
        bool recv(void* payload) override;
        void close() override;
    
    private:
        SocketImpl(boost::asio::io_context& ioContext);

        boost::asio::io_context* m_IOContext;
        boost::asio::ip::tcp::socket m_Socket;
    };
}