#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

// Adapted from https://github.com/progschj/ThreadPool, allows joining of threads without killing pool or getting all futures

class ThreadPool {
public:
	ThreadPool(size_t);
	template<typename F, typename... Args>
	auto Enqueue(F&& f, Args&&... args)
		->std::future<typename std::result_of<F(Args...)>::type>;
	void JoinAll();

private:
	struct Thread {
		std::thread thread;
		bool working;
	};
	// need to keep track of threads so we can join them
	std::vector< std::thread > workers;
	int active = 0;
	// the task queue
	std::queue< std::function<void()> > tasks;

	// synchronization
	std::mutex queue_mutex;
	std::condition_variable workAvailable;
	std::condition_variable workersFinished;
	bool stop;
};

// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads = std::thread::hardware_concurrency())
	: stop(false)
{
	for (size_t i = 0; i<threads; ++i)
		workers.emplace_back(
			[this] {
		for (;;)
		{
			std::function<void()> task;
			{
				std::unique_lock<std::mutex> lock(this->queue_mutex);
				this->workAvailable.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
				if (this->stop && this->tasks.empty()) return;
				task = std::move(this->tasks.front());
				this->tasks.pop();
				++active;
			}
			task();
			{
				std::unique_lock<std::mutex> lock(this->queue_mutex);
				if (!(--active) && this->tasks.empty()) this->workersFinished.notify_one();
			}
		}
	}
	);
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::Enqueue(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type>
{
	using return_type = typename std::result_of<F(Args...)>::type;

	auto task = std::make_shared< std::packaged_task<return_type()> >(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);

	std::future<return_type> res = task->get_future();
	{
		std::unique_lock<std::mutex> lock(queue_mutex);

		// don't allow enqueueing after stopping the pool
		if (stop)
			throw std::runtime_error("enqueue on stopped ThreadPool");

		tasks.emplace([task]() { (*task)(); });
	}
	workAvailable.notify_one();
	return res;
}

void ThreadPool::JoinAll() {
	std::unique_lock<std::mutex> lock(queue_mutex);
	this->workersFinished.wait(lock);
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
	}
	workAvailable.notify_all();
	for (std::thread &worker : workers)
		worker.join();
}