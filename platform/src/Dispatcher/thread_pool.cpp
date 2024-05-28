#include "thread_pool.h"

#include <mutex>
#include <thread>

void Threadpool::Init(int num)
{
    std::call_once(m_once, [this, num]()
                   {
        wlock lock(m_mutex);
        m_stop = false;
        m_cancel = false;
        m_workers.reserve(num);
        for (int i = 0; i < num; ++i) {
            m_workers.emplace_back(std::bind(&Threadpool::Spawn, this));
        }
        m_inited = true; });
}

void Threadpool::Spawn()
{
    for (;;)
    {
        bool pop = false;
        std::function<void()> task;
        {
            wlock lock(m_mutex);

            /**
             Why do we need to lock a mutex with our cv?

            If we didn't lock then what happens if scheduler decides to switch context from this thread
            to a different thread after we checked the condition and decided that it is not time to stop waiting?

            The other thread might make a changes that would otherwise trigger the cv condition to indicate that
            we need to stop waiting. Switching back to the first thread, these changes would go unnoticed (because
            we already run the condition) and it would still think we should go back to continue sleeping, possibly
            forever.
             */
            m_cond.wait(lock, [this, &pop, &task]
                        {
                pop = m_tasks.Pop(task);
                bool should_stop_waiting = m_cancel || m_stop || pop;
                return should_stop_waiting; });
        }
        if (m_cancel || (m_stop && !pop))
        {
            return;
        }

        task();
    }
}

void Threadpool::Terminate()
{
    {
        wlock lock(m_mutex);
        if (IsRunning())
        {
            m_stop = true;
        }
        else
        {
            return;
        }
    }
    m_cond.notify_all();
    for (auto &worker : m_workers)
    {
        worker.join();
    }
}

void Threadpool::Cancel()
{
    {
        wlock lock(m_mutex);
        if (IsRunning())
        {
            m_cancel = true;
        }
        else
        {
            return;
        }
    }
    m_tasks.Clear();
    m_cond.notify_all();
    for (auto &worker : m_workers)
    {
        worker.join();
    }
}
bool Threadpool::Initialized() const
{
    rlock lock(m_mutex);
    return m_inited;
}

bool Threadpool::IsRunning() const
{
    return m_inited && !m_stop && !m_cancel;
}

int Threadpool::Size() const
{
    rlock lock(m_mutex);
    return m_workers.size();
}