#include "dispatcher_builder.h"
#include "dispatcher.h"

DispatcherBuilder::DispatcherBuilder() {}

DispatcherBuilder &DispatcherBuilder::WithName(const std::string &name)
{
    m_name = name;
    return *this;
}

std::unique_ptr<Dispatcher> DispatcherBuilder::Build() const
{
    if (m_name.empty())
    {
        throw std::runtime_error("No name provided");
    }

    std::unique_ptr<Dispatcher> instance = std::make_unique<Dispatcher>(m_name);
    return instance;
}
