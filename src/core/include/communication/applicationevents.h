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
        // my host -----------------
        CONNECTION_ESTABLISHED,
        CONNECTION_CLOSED,
        HOST_DATA_RECEIVED,
        // other hosts 
        HOST_CONNECTED,
        HOST_DISCONNECTED,
        // -------------------------
        EXIT_APPLICATION,
    };
    

    struct ApplicationEvent
    {
        virtual ~ApplicationEvent() = default;

        EventType type = EventType::NONE;
    };

    struct ExitApplication : public ApplicationEvent
    {
        ExitApplication() { type = EventType::EXIT_APPLICATION; }
    };

    struct ConnectionEstablished : public ApplicationEvent
    {
        ConnectionEstablished() { type = EventType::CONNECTION_ESTABLISHED; }

        uint32_t hostID;
    };

    struct ConnectionClosed : public ApplicationEvent
    {
        ConnectionClosed() { type = EventType::CONNECTION_CLOSED; }
    };

    struct HostConnected : public ApplicationEvent
    {
        HostConnected() { type = EventType::HOST_CONNECTED; }

        std::string username;
        uint32_t hostID;
    };

    struct HostDisconnected : public ApplicationEvent
    {
        HostDisconnected() { type = EventType::HOST_DISCONNECTED; }

        uint32_t hostID;
    };

    struct HostDataReceived : public ApplicationEvent
    {
        HostDataReceived() { type = EventType::HOST_DATA_RECEIVED; }

        uint32_t myHostID; // SHOULD: if this data pack gets bigger, make a HostData struct that contains it
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

        messages::MessageID msgID; // = messages::MessageID::MESSAGE_SENT;
    };

    struct ServerChosen : public ApplicationEvent
    {
        ServerChosen() { type = EventType::SERVER_CHOSEN; }

        std::string address;
        std::string port;
        std::string username;
    };

}
