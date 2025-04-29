#pragma once

#include "applicationevents.h"
#include <functional>
#include <map>

namespace core
{
    using EventCallback = std::function<void(ApplicationEvent const&)>;

    class Broadcaster
    {
    public:
        Broadcaster();

        void subscribeToEvent(EventType eventType, EventCallback const& callback);
        void pushEvent(ApplicationEvent const& event);
    
    private:
        std::map<EventType, std::vector<EventCallback>> m_CallbacksMap;
    };
}
