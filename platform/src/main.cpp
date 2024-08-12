
#include <iostream>
#include <string>
#include <unordered_map>
#include <signal.h>
#if __APPLE__
#include <sys/event.h> // for kqueue() etc.
#endif
#include "config.h"
#include "Dispatcher/dispatcher.h"
#include "Dispatcher/dispatcher_builder.h"
#include "Strategies/python_strategy.cpp"
#include "Common/signal_channel.h"
#include "Logging/logging.h"
#include "Api/api_builder.h"

/**
 * Create a return a shared channel for SIGINT signals.
 *
 */
std::shared_ptr<SignalChannel> listen_for_sigint(sigset_t &sigset)
{
    std::shared_ptr<SignalChannel> sig_channel = std::make_shared<SignalChannel>();

#ifdef __linux__
    // Listen for sigint event line Ctrl^c
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &sigset, nullptr);

    std::thread signal_handler{[&sig_channel, &sigset]()
                               {
                                   int signum = 0;

                                   // wait untl a signal is delivered
                                   sigwait(&sigset, &signum);
                                   sig_channel->m_shutdown_requested.store(true);

                                   // notify all waiting workers to check their predicate
                                   sig_channel->m_cv.notify_all();
                                   std::cout << "Received signal " << signum << "\n";
                                   return signum;
                               }};
    signal_handler.detach();
#elif __APPLE__
    std::thread signal_handler{[&sig_channel]()
                               {
                                   int kq = kqueue();

                                   /* Two kevent structs */
                                   struct kevent *ke = (struct kevent *)malloc(sizeof(struct kevent));

                                   /* Initialise struct for SIGINT */
                                   signal(SIGINT, SIG_IGN);
                                   EV_SET(ke, SIGINT, EVFILT_SIGNAL, EV_ADD, 0, 0, NULL);

                                   /* Register for the events */
                                   if (kevent(kq, ke, 1, NULL, 0, NULL) < 0)
                                   {
                                       perror("kevent");
                                       return false;
                                   }

                                   memset(ke, 0x00, sizeof(struct kevent));

                                   // Camp here for event
                                   if (kevent(kq, NULL, 0, ke, 1, NULL) < 0)
                                   {
                                       perror("kevent");
                                   }

                                   switch (ke->filter)
                                   {
                                   case EVFILT_SIGNAL:
                                       std::cout << "Received signal " << strsignal(ke->ident) << "\n";
                                       sig_channel->m_shutdown_requested.store(true);
                                       sig_channel->m_cv.notify_all();
                                       break;
                                   default:
                                       break;
                                   }

                                   return true;
                               }};
    signal_handler.detach();
#endif

    return sig_channel;
}

int main(int argc, char *argv[])
{

    std::cout << argv[0]
              << " Version "
              << MyApp_VERSION_MAJOR
              << "."
              << MyApp_VERSION_MINOR
              << std::endl;

    /*************************************************************************
     *
     * SIGINT CHANNEL
     *
     *************************************************************************/
    sigset_t sigset;
    std::shared_ptr<SignalChannel> signal_channel = listen_for_sigint(sigset);

    /*************************************************************************
     *
     * LOGGER
     *
     *************************************************************************/
    Logging::LogProcessor log_processor("Logger");

    /*************************************************************************
     *
     * DISPATCHER
     *
     *************************************************************************/
    std::shared_ptr<Dispatcher> dispatcher = DispatcherBuilder()
                                                 .WithName("Dispatcher")
                                                 .Build();

    /*************************************************************************
     *
     * API
     *
     *************************************************************************/
    std::unique_ptr<Api> api = ApiBuilder()
                                   .WithName("Api")
                                   .WithHost("http://localhost")
                                   .WithPort(8080)
                                   .WithDispatcher(dispatcher)
                                   .Build();

    while (true)
    {
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(1s);

        if (signal_channel->m_shutdown_requested.load())
        {
            break;
        }
    }

    Logging::INFO("Stopping", "Main");

    return 0;
}