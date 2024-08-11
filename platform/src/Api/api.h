#ifndef API_H
#define API_H

#include "../Dispatcher/dispatcher.h"

#include <atomic>
#include <thread>
#include <memory>

class Api
{
public:
    Api(const std::string &name, const std::string &m_host, const int port, std::shared_ptr<Dispatcher> dispatcher);
    void Run();
    ~Api();

    Api() = delete;
    Api(const Api &) = delete;
    Api(const Api &&) = delete;
    Api &operator=(const Api &) = delete;
    Api &operator=(const Api &&) = delete;

private:
    std::string m_name;
    std::string m_host;
    int m_port = -1;
    std::shared_ptr<Dispatcher> m_dispatcher;
    std::atomic<bool> m_running = {true};
    std::thread *m_thread = nullptr;
};

#endif