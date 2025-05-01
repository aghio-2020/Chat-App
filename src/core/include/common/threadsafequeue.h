#pragma once

#include <queue>
#include <mutex>

namespace core::utils
{
	template <typename T>
	class TSQueue
	{
	public:
		void push(T const& element)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push(element);
		}

		void pop()
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.pop();
		}

		T& front()
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.front();
		}

		bool empty() const
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.empty();
		}

		std::size_t size() const
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.size();
		}

	private:
		std::queue<T> m_Queue;
		// make mutable to avoid breaking the const functions
		mutable std::mutex m_Mutex;
	};
}