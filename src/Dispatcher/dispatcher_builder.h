#ifndef DISPATCHER_BUILDER_H
#define DISPATCHER_BUILDER_H

#include "dispatcher.h"
#include "../Common/signal_channel.h"

class DispatcherBuilder
{
private:
    std::shared_ptr<SignalChannel> m_sig_channel;
    std::string m_name;

public:
    DispatcherBuilder(std::string name);
    DispatcherBuilder &WithSignalChannel(std::shared_ptr<SignalChannel> sc);
    std::unique_ptr<Dispatcher> Build() const;
};

#endif