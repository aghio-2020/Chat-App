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
    
    struct Event
    {
        virtual ~Event() {};
        EventType getEventType() const { return type; }
    protected:
        EventType type = EventType::NONE;
    };

    struct TestEvent : public Event
    {
        TestEvent(int dat) : data(dat)    
        { 
            type = TEST_EVENT; 
        }

        int data = 0;
    };
}
