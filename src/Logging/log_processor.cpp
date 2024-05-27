#include "logging.h"
#include "../Common/thread_utils.h"

SafeQueue<std::string> log_queue;

Logging::LogProcessor::LogProcessor()
{
    // Logging::configure({{"type", "file"}, {"file_name", "app.log"}, {"reopen_interval", "1"}});
    Logging::configure({{"type", "std_out"}});
    // Logging::configure({{"type", "daily"}, {"file_name", "logs/app.log"}, {"hour", "2"}, {"minute", "30"}});

    m_thread = CreateAndStartThread(-1, "LogProcessor", [this]()
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
}

Logging::LogProcessor::~LogProcessor()
{
    std::cout << "Dispatcher shutting down" << std::endl;
    while (log_queue.Size())
    {
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    m_running = false;
    m_thread->join();
}