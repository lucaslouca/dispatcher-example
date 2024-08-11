#ifndef API_H
#define API_H

#include <atomic>
#include <thread>

class Api
{
public:
    Api();
    void Run();
    ~Api();

    Api(const Api &) = delete;
    Api(const Api &&) = delete;
    Api &operator=(const Api &) = delete;
    Api &operator=(const Api &&) = delete;

private:
    std::atomic<bool> m_running = {true};
    std::thread *m_thread = nullptr;
};

#endif