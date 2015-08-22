/*****************************************************************************

 Copyright (c) 2014 RE Game Studio. All rights reserved.

 racing365game@gmail.com

******************************************************************************/

#ifndef NAMEDFACTORY_H
#define NAMEDFACTORY_H

#include <string>
#include <functional>
#include <unordered_map>
#include <memory>

template<typename T>
class NamedFactory
{
    using InstPtr = std::shared_ptr<T>;

public:
    InstPtr createInstance(const std::string& name);

    void registerCreateFunc(const std::string& name, std::function<InstPtr()> func);

private:
    std::unordered_map<std::string, std::function<InstPtr()>> func_map;
};

template<typename T>
inline std::shared_ptr<T> NamedFactory<T>::createInstance(const std::string& name) {
    auto func = func_map.at(name);

    auto inst = func();

    return inst;
}

template<typename T>
inline void NamedFactory<T>::registerCreateFunc(const std::string& name, std::function<InstPtr()> func) {
    if (func_map.count(name) == 0) {
        func_map[name] = func;
    } else {
        // log the error
    }
}

#endif // NAMEDFACTORY_H
