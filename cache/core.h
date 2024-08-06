#ifndef CORE_H
#define CORE_H

#include <string>
#include <map>

class BaseRecord
{
public:
    virtual ~BaseRecord() = default;
    virtual std::string getTypeName() const = 0;
};

template <typename T>
class Record : public BaseRecord
{
    T data;

public:
    int ttl;
    Record(T);
    T get() const;
    void set(T);
    std::string getTypeName() const override;
};

class Cache
{
public:
    BaseRecord *get(std::string);
    void set(std::string, BaseRecord *);
    std::map<std::string, BaseRecord *> table;
};

#endif
