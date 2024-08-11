#include "cache/core.h"


#include <chrono>
#include <cxxabi.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>

template <typename T>
Record<T>::Record(T data) : data(data)
{
    if constexpr (std::is_same_v<T, int>)
    {
        type = INT;
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        type = STRING;
    }
    else if constexpr (std::is_same_v<T, float>)
    {
        type = FLOAT;
    }
    ttl = FLAGS_ttl;
    timestamp = std::chrono::system_clock::now();
}

template <typename T>
Record<T>::Record(T data, int ttl) : data(data), ttl(ttl)
{
    if constexpr (std::is_same_v<T, int>)
    {
        type = INT;
    }
    else if constexpr (std::is_same_v<T, std::string>)
    {
        type = STRING;
    }
    else if constexpr (std::is_same_v<T, float>)
    {
        type = FLOAT;
    }
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

Cache::Cache() : num_keys(0)
{
    reconstruct();
}

BaseRecord *Cache::get(std::string key) const
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

void Cache::reconstruct()
{
    std::filesystem::path log_directory = FLAGS_log_path;
    if (std::filesystem::exists(log_directory) && std::filesystem::is_directory(log_directory))
    {
        for (const auto &entry : std::filesystem::directory_iterator(log_directory))
        {
            if (std::filesystem::is_regular_file(entry.path()) && (entry.path().filename() == FLAGS_log_file))
            {
                std::string absolute_log_file_path = std::string(FLAGS_log_path) + "/" + std::string(FLAGS_log_file);
                std::ifstream file(absolute_log_file_path);
                if (file)
                {
                    std::string line;
                    while (std::getline(file, line))
                    {
                        std::string type,key,data,ttl;
                        int i=0;
                        while( line[i] != ' ')
                        {
                            type += line[i];
                            i++;
                        }
                        i++;
                        while ( line[i] != ' ')
                        {
                            key += line[i];
                            i++;
                        }
                        i++;
                        while( line[i] != ' ')
                        {
                            data += line[i];
                            i++;
                        }
                        i++;
                        while( line[i] != ' ')
                        {
                            ttl += line[i];
                            i++;
                        }
                        int timeToLive = std::stoi(ttl);
                        if ( timeToLive < 0)
                        {
                            continue;
                        }
                        if (type == "INT" )
                        {
                            int insertValue = std::stoi(data);
                            Record<int> val(insertValue);
                            Cache::set(key, &val);                           
                        }
                        if (type == "FLOAT")
                        {
                            float insertValue = std::stof(data);
                            Record<float> val(insertValue);
                            Cache::set(key, &val);
                        }
                        if (type == "STRING"){
                            std::string insertValue = data;
                            Record<std::string> val(insertValue);
                            Cache::set(key, &val);
                        }
                    }
                    std::cout << "Cache reconstructed from the log file.\n";
                    file.close();
                    return;
                }
                else
                {
                    std::cout << "Unable to open the log file.\n";
                }
                return;
            }
        }
    }
    else
    {
        std::cout << "Unable to use the specified log directory path.\n";
        return;
    }
}

template class Record<int>;
template class Record<float>;
template class Record<std::string>;
template class Record<int *>;
