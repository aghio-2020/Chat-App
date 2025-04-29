#pragma once

#define CAST_EVENT(event, eventType)                          \
{                                                             \
    eventType* castedEvent = static_cast<eventType*>(&event); \
}

namespace core
{
    enum EventType
    {
        NONE,
        INPUT_EVENT,
        CONNECTION_ESTABLISHED,
        MESSAGE_SENT,
        EXIT_APPLICATION,
        TEST_EVENT,
    };
    

    struct ApplicationEvent
    {
    public:
        virtual ~ApplicationEvent() = default;

        EventType getEventType() const { return type; }

    protected:
        EventType type = EventType::NONE;
    };
}
