#include "python_strategy.h"
#include "../Logging/logging.h"
#include <Python.h>
#include "Playground.h"

REGISTER_DEF_TYPE(PythonStrategy, python);

void PythonStrategy::Run(const std::string &query, const std::string &parameters) const
{
    Logging::INFO("Running:" + query, "PythonStrategy");

    char *pResult = call_do_work(query.c_str(), parameters.c_str());

    Logging::INFO("Result from Python call: " + std::string(pResult), "PythonStrategy");
}
