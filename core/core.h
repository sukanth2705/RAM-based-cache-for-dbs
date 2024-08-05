#ifndef CORE_H

#define CORE_H
#include <string>
#include <map>

namespace core
{
    class BaseRecord
    {
    public:
        virtual ~BaseRecord() = default;
        virtual std::string getTypeName() const = 0;
    };  
    
    template <typename T>
    class Record : public BaseRecord
    {
        int ttl;
        T data;

    public:
        Record(T);
        T get() const;
        void set(T);
        std::string getTypeName() const override;
    };

    class Cache
    {
        std::map<std::string, BaseRecord *> table;

    public:
        BaseRecord *get(std::string);
        void set(std::string, BaseRecord *);
    };

}
#endif
