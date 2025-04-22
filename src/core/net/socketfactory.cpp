#include "socketfactory.h"
#include "socketimpl.h"

namespace core
{
    SocketFactory::SocketFactory()
    {}

    std::unique_ptr<ISocket> SocketFactory::createSocket()
    {
        return std::make_unique<SocketImpl>(m_IOContext);
    }
}