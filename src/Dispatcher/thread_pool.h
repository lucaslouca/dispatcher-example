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

// https://www.sobyte.net/post/2022-05/design-a-thread-pool/
class Threadpool
{
public:
    using wlock = std::unique_lock<std::shared_mutex>;
    using rlock = std::shared_lock<std::shared_mutex>;

public:
    Threadpool() = default;
    ~Threadpool();
    Threadpool(const Threadpool &) = delete;
    Threadpool(Threadpool &&) = delete;
    Threadpool &operator=(const Threadpool &) = delete;
    Threadpool &operator=(Threadpool &&) = delete;

public:
    void Init(int num);
    void Terminate(); // stop and process all delegated tasks
    void Cancel();    // stop and drop all tasks remained in queue

public:
    bool Initialized() const;
    bool IsRunning() const;
    int Size() const;

public:
    template <class F, class... Args>
    auto Async(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        using return_t = decltype(f(args...));
        using future_t = std::future<return_t>;
        using task_t = std::packaged_task<return_t()>;

        {
            rlock lock(m_mutex);
            if (m_stop || m_cancel)
            {
                throw std::runtime_error("Delegating task to a threadpool that has been terminated or canceled.");
            }
        }

        auto bind_func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        std::shared_ptr<task_t> task = std::make_shared<task_t>(std::move(bind_func));
        future_t fut = task->get_future();
        m_tasks.Emplace([task]() -> void
                        { (*task)(); });
        m_cond.notify_one();
        return fut;
    }

    void Spawn();

private:
    std::vector<std::thread> m_workers;
    mutable TasksQueue<std::function<void()>> m_tasks;
    bool m_stop{false};
    bool m_cancel{false};
    bool m_inited{false};
    mutable std::shared_mutex m_mutex;
    std::condition_variable_any m_cond;
    mutable std::once_flag m_once;
};

#endif