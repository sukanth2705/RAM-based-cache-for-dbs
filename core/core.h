#ifndef CORE_H
 
#define CORE_H
 
namespace core {
    enum TYPES {
        INT,
        FLOAT,
    };

    class Record {
        private:
            TYPES t;
            int ttl;
    };

}
#endif