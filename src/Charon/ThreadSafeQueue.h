#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H


#include <deque>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>

namespace Charon {

	template<typename T>
	class ThreadSafeQueue
	{
	public:
		ThreadSafeQueue() :
			m_isForceWakeup(false)
		{
		}
		
		ThreadSafeQueue(const ThreadSafeQueue&) = delete; //no copy
		~ThreadSafeQueue() { Clear(); }

		void PushBack(const T& data)
		{
			std::lock_guard<std::mutex> guard(m_queueMutex);
			m_queue.push_back(data);

			std::lock_guard<std::mutex> condGuard(m_conditionMutex);
			m_conditional.notify_one();
		}

		void PushFront(const T& data)
		{
			std::lock_guard<std::mutex> guard(m_queueMutex);
			m_queue.push_front(data);

			std::lock_guard<std::mutex> conditionGuard(m_conditionMutex);
			m_conditional.notify_one();
		}

		void PopBack()
		{
			std::lock_guard<std::mutex> guard(m_queueMutex);
			m_queue.pop_back();
		}

		void PopFront()
		{
			std::lock_guard<std::mutex> guard(m_queueMutex);
			m_queue.pop_front();
		}

		const T& Front()
		{
			std::lock_guard<std::mutex> guard(m_queueMutex);
			return m_queue.front();
		}

		const T& Back()
		{
			std::lock_guard<std::mutex> guard(m_queueMutex);
			return m_queue.back();
		}

		std::size_t Size()
		{
			std::lock_guard<std::mutex> guard(m_queueMutex);
			return m_queue.size();
		}

		bool IsEmpty()
		{
			std::lock_guard<std::mutex> guard(m_queueMutex);
			return m_queue.empty();
		}

		void Clear()
		{
			std::lock_guard<std::mutex> guard(m_queueMutex);
			m_queue.clear();
		}

		//For iterator loops, need begin()/end() idiom

		typename std::deque<T>::iterator begin()
		{
			std::lock_guard<std::mutex> guard(m_queueMutex);
			return m_queue.begin();
		}

		typename std::deque<T>::iterator end()
		{
			std::lock_guard<std::mutex> guard(m_queueMutex);
			return m_queue.end();
		}

		void Wait()
		{
			while (IsEmpty() && !m_isForceWakeup)
			{
				std::unique_lock<std::mutex> guard(m_conditionMutex);
				m_conditional.wait(guard);
			}
		}

		void ForceWakeup()
		{
			m_isForceWakeup = true;

			std::unique_lock<std::mutex> guard(m_conditionMutex);
			m_conditional.notify_one();
		}

		void ResetWakeup()
		{
			m_isForceWakeup = false;
		}

	private:
		std::mutex m_queueMutex;
		std::deque<T> m_queue;

		std::mutex m_conditionMutex;
		std::condition_variable m_conditional;

		std::atomic<bool> m_isForceWakeup;
	};
}

#endif