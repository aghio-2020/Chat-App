#pragma once

#include <boost/asio.hpp>
#include <net/isocket.h>

#include <memory>

namespace core
{
    class SocketFactory
    {
    public:
        SocketFactory();

        std::unique_ptr<ISocket> createSocket();

    private:
        std::shared_ptr<boost::asio::io_context> m_IOContext;
    };
}