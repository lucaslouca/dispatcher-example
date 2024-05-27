#include "python_strategy.h"

REGISTER_DEF_TYPE(PythonStrategy, python);

void PythonStrategy::Run(const std::string &query, const std::unordered_map<std::string, std::string> &parameters) const
{
    std::cout << "PythonStrategy:" << query << std::endl;
}
