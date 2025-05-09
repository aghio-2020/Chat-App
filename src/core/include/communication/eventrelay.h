#pragma once

#include <string>
#include <mutex>

namespace core::events
{
    // interface to wrap a class of events that can be passed as reference somewhere else or inherit where they are needed.
    // the goal of this was to avoid instantiating callbacks everywhere where we needed to hear a certain event

    // COULD: 
    // instead make a context global to each component with all the callbacks from the component you want to 
    // expose to classes used in its implementation by getting the static instance of it

    template<class EventClass>
    class EventRelay
    {
    public:
        EventClass& getRelay()
        {
            std::scoped_lock lock(m_Mutex);
            return m_Instance;
        }

    private:
        EventClass m_Instance;
        std::mutex m_Mutex;
    };
}