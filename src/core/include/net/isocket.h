#pragma once

#include <string>

namespace core
{
    class ISocket
    {
    public:
        virtual ~ISocket() = default;
    
        virtual bool connect(std::string const& addr, std::string const& port) = 0;
        virtual bool send(void* payload) = 0;
        virtual bool recv(void* payload) = 0;
        virtual void close() = 0;
    };
}
