#ifndef CORE_H
 
#define CORE_H
 
namespace core {
    enum TYPES {
        INT,
        FLOAT,
    };

    class Record {
        TYPES t;
        int ttl;
    };

    class Int: public Record {
        int data;
        int get();
        void set(int );
    };
}
#endif