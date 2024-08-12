#ifndef PYTHON_STRATEGY_H
#define PYTHON_STRATEGY_H

#include <iostream>
#include "strategy.h"
#include "strategy_factory.h"

class PythonStrategy : public Strategy
{
private:
    /*
    static: Dynamic initialization of an object with static storage duration is guaranteed to
    happen before execution of any function defined in the same translation unit. If there are
    no such functions, or your program never calls them, then there's no guarantee it will ever
    be initialised.

    static DecoratorRegister<DecoratorUnuuid> reg;
    */
    REGISTER_DEC_TYPE(PythonStrategy);

public:
    PythonStrategy() {}

    void Run(const std::string &query, const std::string &parameters) const override;

    virtual ~PythonStrategy()
    {
    }
};

#endif