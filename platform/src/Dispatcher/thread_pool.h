#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>
#include "tasks_queue.h"

class ThreadPool
{
public:
    ThreadPool(unsigned int threads = std::thread::hardware_concurrency()) : m_queue(), m_count(threads)
    {
        assert(threads != 0);
        auto worker = [&](unsigned int i)
        {
            while (true)
            {
                Proc f;
                if (m_queue.Pop(f))
                {
                    f();
                }
                else
                {
                    std::this_thread::yield();
                }
            }
        };
        for (unsigned int i = 0; i < threads; ++i)
            m_threads.emplace_back(worker, i);
    }

    ~ThreadPool() noexcept
    {
        for (auto &thread : m_threads)
        {
            thread.join();
        }
    }

    template <typename F, typename... Args>
    void Submit(F &&f, Args &&...args)
    {
        auto work = [f, args...]()
        {
            f(args...);
        };

        m_queue.Push(work);
    }

private:
    using Proc = std::function<void(void)>;
    using Threads = std::vector<std::thread>;

    TasksQueue<Proc> m_queue;
    Threads m_threads;
    const unsigned int m_count;
    std::atomic_uint m_index = 0;
    inline static const unsigned int K = 3;
};

#endif