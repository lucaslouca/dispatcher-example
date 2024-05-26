#include "dispatcher_builder.h"
#include "dispatcher.h"

DispatcherBuilder::DispatcherBuilder(std::string name) : m_name(name) {}

DispatcherBuilder &DispatcherBuilder::WithSignalChannel(std::shared_ptr<SignalChannel> sc)
{
    m_sig_channel = sc;
    return *this;
}

std::unique_ptr<Dispatcher> DispatcherBuilder::Build() const
{
    if (!m_sig_channel)
    {
        throw std::runtime_error("No signal channel provided");
    }

    std::unique_ptr<Dispatcher> instance = std::make_unique<Dispatcher>(m_name, m_sig_channel);
    return instance;
}
