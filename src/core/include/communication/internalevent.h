#pragma once

#include <functional>
#include <type_traits>

namespace core::events
{
	template <typename ...Args>
	class InternalEvent
	{
	public:
		using InternalCallback = std::function<void(Args...)>;

		void subscribe(InternalCallback const& cb)
		{
			m_Callback = cb;
		}

		void operator()(Args... args)
		{
			if (m_Callback)
			{
				// std::forward allows detecting if one of the Arg types is an rvalue.
				m_Callback(std::forward<Args>(args)...);
			}
		}

	private:
		InternalCallback m_Callback;
	};
}