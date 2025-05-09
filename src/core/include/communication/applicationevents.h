#pragma once

#include <string>
#include <chrono>

#include <net/message.h>

#define CAST_EVENT(event, eventType)                          \
{                                                             \
    eventType* castedEvent = static_cast<eventType*>(&event); \
}

namespace core::events
{
    enum EventType
    {
        NONE,
        INPUT_EVENT,
        MESSAGE_SENT,
        CHAT_MESSAGE_RECEIVED,
        CHAT_MESSAGE_POSTED,
        SERVER_CHOSEN,
        CONNECTION_ESTABLISHED,
        HOST_DISCONNECTED,
        EXIT_APPLICATION,
    };
    

    struct ApplicationEvent
    {
        virtual ~ApplicationEvent() = default;

        EventType type = EventType::NONE;
    };

    struct ConnectionEstablished : public ApplicationEvent
    {
        ConnectionEstablished() { type = EventType::CONNECTION_ESTABLISHED; }

        uint32_t hostID;
    };

    struct HostDisconnected : public ApplicationEvent
    {
        HostDisconnected() { type = EventType::HOST_DISCONNECTED; }

        uint32_t hostID;
    };

    struct ChatMessageReceived : public ApplicationEvent
    {
        ChatMessageReceived() { type = EventType::CHAT_MESSAGE_RECEIVED; }

        messages::ChatMessage msg;
    };

    struct ChatMessagePosted : public ApplicationEvent
    {
        ChatMessagePosted() { type = EventType::CHAT_MESSAGE_POSTED; }

        messages::ChatMessage msg;
    };

    // a message was written into the socket successfully
    struct MessageSent : public ApplicationEvent
    {
        MessageSent() { type = EventType::MESSAGE_SENT; }

        messages::MessageID msgID;
    };

    struct ServerChosen : public ApplicationEvent
    {
        ServerChosen() { type = EventType::SERVER_CHOSEN; }

        std::string address;
        std::string port;
    };

}
