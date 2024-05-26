
#include <iostream>
#include <Python.h>
#include <string>
#include <unordered_map>
#include <signal.h>
#if __APPLE__
#include <sys/event.h> // for kqueue() etc.
#endif
#include "MyAppConfig.h"
#include "Playground.h"
#include "Dispatcher/dispatcher.h"
#include "Dispatcher/dispatcher_builder.h"
#include "Common/signal_channel.h"

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

    PyImport_AppendInittab("Playground", PyInit_Playground);
    Py_Initialize();
    PyImport_ImportModule("Playground");

    PyObject *pNames;
    if (!(pNames = PyList_New(0)))
    {
        return 1;
    }
    for (int i = 1; i <= 3; i++)
    {
        std::string n = "Name_" + std::to_string(i);
        PyList_Append(pNames, PyUnicode_FromString(n.c_str()));
    }

    char *pResult = call_hello_world("Hello", pNames);
    std::cout << "Result from Python call: " << pResult << std::endl;
    Py_Finalize();

    std::unique_ptr<Dispatcher> dispatcher = DispatcherBuilder("Dispatcher").WithSignalChannel(signal_channel).Build();

    while (true)
    {
        std::unordered_map<std::string, std::string> params = {{"key1", "value1"}, {"key2", "value2"}};
        dispatcher->Dispatch("Location", params);

        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(1s);

        if (signal_channel->m_shutdown_requested)
        {
            break;
        }
    }

    std::cout << "Main stoping" << std::endl;

    return 0;
}