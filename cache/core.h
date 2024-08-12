#ifndef CORE_H
#define CORE_H

#include "flags.h"

#include <chrono>
#include <string>
#include <map>
#include <vector>
#include <set>

enum TYPE
{
    INT,
    FLOAT,
    STRING,
};

enum COMMANDS
{
    SET,
    GET,
    PING,
};

class BaseRecord
{
public:
    TYPE type;
    virtual ~BaseRecord() = default;
    virtual std::string getTypeName() const = 0;
};

template <typename T>
class Record : public BaseRecord
{
    T data;

public:
    int ttl;
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    Record(T);
    Record(T, int);
    T get() const;
    void set(T);
    std::string getTypeName() const override;
};

class Cache
{
    void reconstruct();

public:
    std::map<std::string, BaseRecord *> table;
    std::set<std::string> keys;
    int num_keys;
    BaseRecord *get(std::string) const;
    void set(std::string, BaseRecord *);
    Cache();
};

#endif
