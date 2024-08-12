#ifndef STRATEGY_H
#define STRATEGY_H

#include <unordered_map>
#include <string>
class Strategy
{

public:
    Strategy() {};
    virtual void Run(const std::string &query, const std::string &parameters) const = 0;
    virtual ~Strategy() {}
};

#endif