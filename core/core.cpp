#include "core.h"
#include <cxxabi.h>
#include <iostream>
#include <string>
#include <typeinfo>

namespace core
{
    template <typename T>
    Record<T>::Record(T data) : data(data) {}

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
    }

    template class Record<int>;
    template class Record<float>;
    template class Record<std::string>;
    template class Record<int *>;
}

int main()
{
    std::string a = "abcd";
    core::Record<std::string> v(a);
    std::cout << v.getTypeName() << std::endl;
    core::Cache cache;
    cache.set("abc", &v);
    core::Record<std::string> *val = dynamic_cast<core::Record<std::string> *>(cache.get("abc"));
    std::cout << val->get() << std::endl;
    std::cout << val->getTypeName() << std::endl;
    return 0;
}