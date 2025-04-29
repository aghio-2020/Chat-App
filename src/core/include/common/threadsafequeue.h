#include <queue>
#include <mutex>

namespace core::utils
{
	template <typename T>
	class TSQueue
	{
	public:
		void push_back(T const& element)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_back(element);
		}

		T pop_front()
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.pop_front();
		}

		T& front() const
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.front();
		}

		bool empty() const
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.empty();
		}

	private:
		std::queue<T> m_Queue;
		std::mutex m_Mutex;
	};
}