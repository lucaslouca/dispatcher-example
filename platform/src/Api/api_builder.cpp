#include "api_builder.h"

ApiBuilder::ApiBuilder() {}

ApiBuilder &ApiBuilder::WithName(const std::string &name)
{
    m_name = name;
    return *this;
}

ApiBuilder &ApiBuilder::WithHost(const std::string &host)
{
    m_host = host;
    return *this;
}

ApiBuilder &ApiBuilder::WithPort(const int &port)
{
    m_port = port;
    return *this;
}

ApiBuilder &ApiBuilder::WithDispatcher(std::shared_ptr<Dispatcher> dispatcher)
{
    m_dispatcher = dispatcher;
    return *this;
}

std::unique_ptr<Api> ApiBuilder::Build() const
{
    if (m_name.empty())
    {
        throw std::runtime_error("No name provided");
    }

    if (m_host.empty())
    {
        throw std::runtime_error("No host provided");
    }

    if (m_port == -1)
    {
        throw std::runtime_error("No port provided");
    }

    if (m_dispatcher == nullptr)
    {
        throw std::runtime_error("No dispatcher provided");
    }

    std::unique_ptr<Api> instance = std::make_unique<Api>(m_name, m_host, m_port, m_dispatcher);
    return instance;
}