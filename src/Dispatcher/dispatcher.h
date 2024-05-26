#pragma once

#include <string>
#include <fstream>
#include <cstdio>
#include <utility>
#include "thread_utils.h"
#include "lf_queue.h"
#include "../Common/signal_channel.h"

constexpr size_t DISPATCHER_QUEUE_SIZE = 8 * 1024 * 1024;

class Dispatcher final
{

public:
    void FlushQueue() noexcept
    {
        while (m_running)
        {
            if (m_queue.size())
            {
                auto next = m_queue.getNextToRead();
                m_queue.updateReadIndex();

                auto query_name = std::get<0>(*next);
                std::cout << query_name << std::endl;
            }

            using namespace std::literals::chrono_literals;
            std::this_thread::sleep_for(10ms);
        }

        std::cout << "Dispatcher break" << std::endl;
    }

    Dispatcher(const std::string name, std::shared_ptr<SignalChannel> signal_channel) : m_name(name), m_signal_channel(signal_channel), m_queue(DISPATCHER_QUEUE_SIZE)
    {
        m_thread = CreateAndStartThread(-1, "Dispatcher", [this]()
                                        { FlushQueue(); });
    }

    ~Dispatcher()
    {
        std::cout << "Dispatcher shutting down" << std::endl;
        while (m_queue.size())
        {
            using namespace std::literals::chrono_literals;
            std::this_thread::sleep_for(1s);
        }
        m_running = false;
        m_thread->join();
    }

    auto Dispatch(const std::string &name, const std::unordered_map<std::string, std::string> &params) noexcept
    {
        *(m_queue.getNextToWriteTo()) = std::make_pair(name, params);
        m_queue.updateWriteIndex();
    }

    Dispatcher() = delete;
    Dispatcher(const Dispatcher &) = delete;
    Dispatcher(const Dispatcher &&) = delete;
    Dispatcher &operator=(const Dispatcher &) = delete;
    Dispatcher &operator=(const Dispatcher &&) = delete;

private:
    LFQueue<std::pair<std::string, std::unordered_map<std::string, std::string>>> m_queue;
    std::atomic<bool> m_running = {true};
    std::thread *m_thread = nullptr;
    std::string m_name;
    std::shared_ptr<SignalChannel> m_signal_channel;
};
