#ifndef TASKS_QUEUE_H
#define TASKS_QUEUE_H

#include <mutex>
#include <queue>

template <typename T>
class TasksQueue : protected std::queue<T>
{
public:
    using wlock = std::unique_lock<std::shared_mutex>;
    using rlock = std::shared_lock<std::shared_mutex>;

public:
    TasksQueue() = default;
    ~TasksQueue() { Clear(); }
    TasksQueue(const TasksQueue &) = delete;
    TasksQueue(TasksQueue &&) = delete;
    TasksQueue &operator=(const TasksQueue &) = delete;
    TasksQueue &operator=(TasksQueue &&) = delete;

public:
    bool IsEmpty() const
    {
        rlock lock(m_mutex);
        return std::queue<T>::empty();
    }

    size_t Size() const
    {
        rlock lock(m_mutex);
        return std::queue<T>::size();
    }

public:
    void Clear()
    {
        wlock lock(m_mutex);
        while (!std::queue<T>::empty())
        {
            std::queue<T>::pop();
        }
    }

    void Push(const T &obj)
    {
        wlock lock(m_mutex);
        std::queue<T>::push(obj);
    }

    template <typename... Args>
    void Emplace(Args &&...args)
    {
        wlock lock(m_mutex);
        std::queue<T>::emplace(std::forward<Args>(args)...);
    }

    bool Pop(T &holder)
    {
        wlock lock(m_mutex);
        if (std::queue<T>::empty())
        {
            return false;
        }
        else
        {
            holder = std::move(std::queue<T>::front());
            std::queue<T>::pop();
            return true;
        }
    }

private:
    mutable std::shared_mutex m_mutex;
};

#endif