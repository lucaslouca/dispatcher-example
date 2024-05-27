#ifndef DISPATCHER_BUILDER_H
#define DISPATCHER_BUILDER_H

#include "dispatcher.h"
#include "../Common/signal_channel.h"

class DispatcherBuilder
{
private:
    std::string m_name;

public:
    DispatcherBuilder();
    DispatcherBuilder &WithName(const std::string &name);
    std::unique_ptr<Dispatcher> Build() const;
};

#endif