/**
 * Courtesy of Marius Bancila at: https://mariusbancila.ro/blog/2017/11/19/revisited-full-fledged-client-server-example-with-c-rest-sdk-2-10/
 */
#include "api.h"
#include "../Common/thread_utils.h"

#include <cpprest/json.h>

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <functional>

Api::Api(const std::string &name, const std::string &host, const int port, std::shared_ptr<Dispatcher> dispatcher) : m_name(name),
                                                                                                                     m_host(host),
                                                                                                                     m_port(port),
                                                                                                                     m_dispatcher(dispatcher)
{
    m_thread = CreateAndStartThread(-1, m_name, [this]()
                                    { Run(); });
}

void HandleRequest(
    web::http::http_request request,
    std::function<void(web::json::value const &, web::json::value &)> action)
{
    auto answer = web::json::value::object();

    request
        .extract_json()
        .then([&answer, &action](pplx::task<web::json::value> task)
              {
         try
         {
            auto const & jvalue = task.get();

            if (!jvalue.is_null())
            {
               action(jvalue, answer);
            }
         }
         catch (web::http::http_exception const &e)
         {
            std::cout << e.what() << std::endl;
         } })
        .wait();

    request.reply(web::http::status_codes::OK, answer);
}

void Api::HandlePost(web::http::http_request request)
{
    HandleRequest(
        request,
        [this](web::json::value const &jvalue, web::json::value &answer)
        {
            Logging::INFO("Got json: " + jvalue.serialize(), m_name);
            std::string strategy_name = jvalue.at(U("name")).as_string();
            std::string parameters = jvalue.serialize();
            m_dispatcher->Dispatch(strategy_name, parameters);
        });
}

void Api::Run()
{
    /*
    curl --header "Content-Type: application/json" --request POST --data '{"name":"Location", "parameters": {"user":"fizzbuzzer", "caseId": "1", "term": "Lorem Ipsum"}, "forwards": [{"name": "Location", "parameters": [{"from": "loc", "to": "term"}]}]}' http://localhost:8080/api

    */
    std::string listenAddress = m_host + ":" + std::to_string(m_port) + "/api";
    web::http::experimental::listener::http_listener listener(listenAddress);

    listener.support(web::http::methods::POST, std::bind(&Api::HandlePost, this, std::placeholders::_1));

    try
    {
        listener
            .open()
            .then([&listener, &listenAddress]()
                  { std::cout << "Started listening on: " << listenAddress << std::endl; })
            .wait();

        while (m_running)
        {
        }
    }
    catch (std::exception const &e)
    {
        std::cout << e.what() << std::endl;
    }
}

Api::~Api()
{
    m_running = false;
    m_thread->join();
}