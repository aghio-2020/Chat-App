#pragma once

#include <communication/internalevent.h>
#include <net/message.h>
#include <string>

namespace core::events
{
    class HostEvents
    {
    public:
        // will receive hostID
        InternalEvent<uint32_t> onHostConnection;

        // hostID disconnected
        InternalEvent<uint32_t> onHostDisconnected;

        // will receive the message to be processed and sender hostID
        InternalEvent<uint32_t, messages::Message&> onMessageReceived;

        // notify what message was just sent
        InternalEvent<messages::MessageID> onMessageSent;
    };
}