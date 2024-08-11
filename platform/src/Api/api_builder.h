#ifndef API_BUILDER_H
#define API_BUILDER_H

#include "api.h"
#include "../Dispatcher/dispatcher.h"
#include <string>

class ApiBuilder
{
private:
    std::string m_name;
    std::string m_host;
    int m_port = -1;
    std::shared_ptr<Dispatcher> m_dispatcher = nullptr;

public:
    ApiBuilder();
    ApiBuilder &WithName(const std::string &name);
    ApiBuilder &WithHost(const std::string &host);
    ApiBuilder &WithPort(const int &port);
    ApiBuilder &WithDispatcher(std::shared_ptr<Dispatcher> dispatcher);
    std::unique_ptr<Api> Build() const;
};

#endif