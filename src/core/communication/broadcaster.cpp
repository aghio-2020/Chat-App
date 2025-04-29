#include <communication/broadcaster.h>
#include <mutex>

namespace core
{
	std::mutex g_Mutex;

	Broadcaster::Broadcaster()
	{
	}

	void Broadcaster::subscribeToEvent(EventType eventType, EventCallback const& callback)
	{
		std::lock_guard<std::mutex> lock(g_Mutex);

		if (callback)
		{
			m_CallbacksMap[eventType].emplace_back(callback);
		}
	}

	void Broadcaster::pushEvent(ApplicationEvent const& event)
	{
		std::lock_guard<std::mutex> lock(g_Mutex);

		auto eventCallbacks = m_CallbacksMap.find(event.getEventType());
		if (eventCallbacks != m_CallbacksMap.end())
		{
			for (auto cb : eventCallbacks->second)
			{
				cb(event);
			}
		}
	}
}

