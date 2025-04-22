#pragma once

#include "events.h"
#include <functional>
#include <map>

namespace core
{
    using EventCallback = std::function<void(Event const&)>;

    class Broadcaster
    {
    public:
        Broadcaster();

        void subscribeToEvent(EventType eventType, EventCallback const& callback);
        void pushEvent(Event const& event);
    
    private:
        std::map<EventType, std::vector<EventCallback>> m_CallbacksMap;
    };
}
