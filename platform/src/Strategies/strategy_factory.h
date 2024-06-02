#ifndef STRATEGY_FACTORY_H
#define STRATEGY_FACTORY_H

#include <map>
#include <memory>
#include <string>

#include "strategy.h"
#include "../Logging/logging.h"

#define REGISTER_DEC_TYPE(TYPE_NAME) \
    static StrategyRegister<TYPE_NAME> reg

#define REGISTER_DEF_TYPE(TYPE_NAME, CONFIG_NAME) \
    StrategyRegister<TYPE_NAME> TYPE_NAME::reg(#CONFIG_NAME)

template <typename T>
Strategy *CreateT()
{
    return new T();
}

struct StrategyFactory
{
    template <typename T>
    using MFP = T *(*)();
    using map_type = std::map<std::string, MFP<Strategy>>;

private:
    static map_type *map;

protected:
    static map_type *GetMap()
    {
        // Never deleted. Exists until program termination.
        if (!map)
        {
            map = new map_type;
        }
        return map;
    }

public:
    static Strategy *GetInstance(const std::string &s)
    {
        map_type::iterator it = GetMap()->find(s);
        if (it == GetMap()->end())
        {
            return nullptr;
        }
        else
        {
            return it->second();
        }
    }
};

template <typename T>
struct StrategyRegister : StrategyFactory
{
    StrategyRegister(const std::string &config_name)
    {
        GetMap()
            ->insert(std::make_pair<std::string, MFP<Strategy>>(static_cast<std::string>(config_name), &CreateT<T>));
    }
};

#endif