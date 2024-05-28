#include "logging.h"
#include "../Common/thread_utils.h"

SafeQueue<std::string> log_queue;

Logging::LogProcessor::LogProcessor(const std::string &name) : m_name(name)
{
    // Logging::configure({{"type", "file"}, {"file_name", "app.log"}, {"reopen_interval", "1"}});
    Logging::configure({{"type", "std_out"}});
    // Logging::configure({{"type", "daily"}, {"file_name", "logs/app.log"}, {"hour", "2"}, {"minute", "30"}});

    m_thread = CreateAndStartThread(-1, m_name, [this]()
                                    { FlushQueue(); });
}

void Logging::LogProcessor::FlushQueue() noexcept
{
    while (m_running)
    {
        if (log_queue.Size())
        {
            std::string message;
            log_queue.DequeueWithTimeout(1000, message);

            if (!message.empty())
            {
                Logging::log(message);
            }
        }

        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(10ms);
    }
    std::cout << "Logger beak" << std::endl;
}

Logging::LogProcessor::~LogProcessor()
{
    Logging::INFO("Shutting down", m_name);
    while (log_queue.Size())
    {
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    m_running = false;
    m_thread->join();
}