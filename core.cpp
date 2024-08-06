#include "cache/core.h"
#include <chrono>
#include <cxxabi.h>
#include <iostream>
#include <string>
#include <typeinfo>

template <typename T>
Record<T>::Record(T data) : data(data)
{
    ttl = 5;
    timestamp = std::chrono::system_clock::now();
}

template <typename T>
T Record<T>::get() const { return data; }

template <typename T>
void Record<T>::set(T val) { data = val; }

template <typename T>
std::string Record<T>::getTypeName() const
{
    int status;
    char *realname = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
    std::string typeName(realname);
    std::free(realname);
    return typeName;
}

Cache::Cache(int num_keys) : num_keys(num_keys) {}

BaseRecord *Cache::get(std::string key)
{
    auto it = table.find(key);
    if (it != table.end())
    {
        return it->second;
    }
    return nullptr;
}

void Cache::set(std::string key, BaseRecord *record)
{
    table[key] = record;
    keys.insert(key);
    num_keys++;
}

template class Record<int>;
template class Record<float>;
template class Record<std::string>;
template class Record<int *>;
