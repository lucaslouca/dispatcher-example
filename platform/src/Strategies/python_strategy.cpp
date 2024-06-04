#include "python_strategy.h"
#include "../Logging/logging.h"
#include <Python.h>
#include "Playground.h"

REGISTER_DEF_TYPE(PythonStrategy, python);

void PythonStrategy::Run(const std::string &query, const std::unordered_map<std::string, std::string> &parameters) const
{
    Logging::INFO("Running:" + query, "PythonStrategy");

    PyObject *pParameters = PyDict_New();
    for (const auto &[k, v] : parameters)
    {
        PyObject *key = PyUnicode_FromString(k.c_str());
        PyObject *value = PyUnicode_FromString(v.c_str());
        if (PyDict_SetItem(pParameters, key, value) < 0)
        {
            return;
        }
    }

    char *pResult = call_hello_world(query.c_str(), pParameters);

    Logging::INFO("Result from Python call: " + std::string(pResult), "PythonStrategy");
}
