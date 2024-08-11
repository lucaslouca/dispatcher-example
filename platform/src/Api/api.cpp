/**
 * Courtesy of Marius Bancila at: https://mariusbancila.ro/blog/2017/11/19/revisited-full-fledged-client-server-example-with-c-rest-sdk-2-10/
 */
#include "api.h"
#include "../Common/thread_utils.h"

#include <cpprest/json.h>
#include <cpprest/http_listener.h>

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

void HandlePost(web::http::http_request request)
{
    HandleRequest(
        request,
        [](web::json::value const &jvalue, web::json::value &answer)
        {
            std::cout << jvalue.serialize() << std::endl;
        });
}

void Api::Run()
{
    /*
    curl --header "Content-Type: application/json" --request POST --data '{"username":"xyz","password":"xyz"}' http://localhost:8080/api

    */
    std::string listenAddress = m_host + ":" + std::to_string(m_port) + "/api";
    web::http::experimental::listener::http_listener listener(listenAddress);

    listener.support(web::http::methods::POST, HandlePost);

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