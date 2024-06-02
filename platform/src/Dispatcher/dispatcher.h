#pragma once

#include <string>
#include <fstream>
#include <cstdio>
#include <utility>
#include <Python.h>
#include <thread>
#include "lf_queue.h"
#include "Playground.h"
#include "thread_pool.h"
#include "../Common/thread_utils.h"
#include "../Logging/logging.h"
#include "../Strategies/strategy_factory.h"

constexpr size_t DISPATCHER_QUEUE_SIZE = 8 * 1024 * 1024;

class Dispatcher final
{

public:
    void FlushQueue() noexcept
    {
        PyImport_AppendInittab("Playground", PyInit_Playground);
        Py_Initialize();
        PyImport_ImportModule("Playground");

        while (m_running)
        {
            if (m_queue.Size())
            {
                auto next = m_queue.GetNextToRead();
                m_queue.updateReadIndex();

                auto query_name = std::get<0>(*next);
                auto parameters = std::get<1>(*next);

                Logging::INFO("Dispatching " + query_name, "Dispatcher");

                Strategy *strategy = StrategyFactory::GetInstance("python");

                auto task = [strategy, query_name](std::unordered_map<std::string, std::string> params)
                {
                    strategy->Run(query_name, params);
                };

                m_tpool.Async(task, parameters);
            }

            using namespace std::literals::chrono_literals;
            std::this_thread::sleep_for(10ms);
        }

        Py_Finalize();

        std::cout << "Dispatcher beak" << std::endl;
    }

    Dispatcher(const std::string name) : m_name(name), m_queue(DISPATCHER_QUEUE_SIZE)
    {
        m_thread = CreateAndStartThread(-1, "Dispatcher", [this]()
                                        { FlushQueue(); });
        m_tpool.Init(std::thread::hardware_concurrency());
    }

    ~Dispatcher()
    {
        Logging::INFO("Shutting down", "Dispatcher");
        while (m_queue.Size())
        {
            using namespace std::literals::chrono_literals;
            std::this_thread::sleep_for(1s);
        }
        m_running = false;
        m_thread->join();
    }

    auto Dispatch(const std::string &name, const std::unordered_map<std::string, std::string> &params) noexcept
    {
        *(m_queue.GetNextToWriteTo()) = std::make_pair(name, params);
        m_queue.UpdateWriteIndex();
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
    Threadpool m_tpool;
};
