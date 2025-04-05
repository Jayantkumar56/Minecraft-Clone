

#pragma once


#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
	ThreadPool(size_t threadCount) :
		m_ShutdownRequested(false)
	{
		m_Threads.reserve(static_cast<size_t>(threadCount));
		for (size_t i = 0; i < threadCount; ++i) {
			m_Threads.emplace_back(&ThreadPool::ThreadWorker, this);
		}
	}

	~ThreadPool() {
		{
			std::lock_guard<std::mutex> lock(m_TaskQueueMutex);
			m_ShutdownRequested = true;
			m_TaskQueueCV.notify_all();
		}

		for (size_t i = 0; i < m_Threads.size(); ++i) {
			if (m_Threads[i].joinable()) {
				m_Threads[i].join();
			}
		}
	}

	void AddTask(std::move_only_function<void()>&& task) {
		std::lock_guard<std::mutex> lock(m_TaskQueueMutex);
		m_TaskQueue.emplace(std::move(task));

		// Wake up one thread if its waiting
		m_TaskQueueCV.notify_one();
	}

	void AddTasks(std::vector<std::move_only_function<void()>>& tasks) {
		std::lock_guard<std::mutex> lock(m_TaskQueueMutex);

		for (size_t i = 0; i < tasks.size(); ++i) {
			m_TaskQueue.emplace(std::move(tasks[i]));
		}

		m_TaskQueueCV.notify_all();
	}

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&)      = delete;

	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&)      = delete;

private:
	void ThreadWorker() {
		std::unique_lock<std::mutex> lock(m_TaskQueueMutex);

		while (!m_ShutdownRequested) {
			m_TaskQueueCV.wait(lock, [this] { return this->m_ShutdownRequested || !this->m_TaskQueue.empty(); });

			if (!m_TaskQueue.empty()) {
				std::move_only_function<void()> func = std::move(m_TaskQueue.front());
				m_TaskQueue.pop();

				lock.unlock();
				func();
				lock.lock();
			}
		}
	}

private:
	bool m_ShutdownRequested;

	std::vector<std::thread>          m_Threads;
	std::queue<std::move_only_function<void()>> m_TaskQueue;

	std::mutex              m_TaskQueueMutex;
	std::condition_variable m_TaskQueueCV;
};
