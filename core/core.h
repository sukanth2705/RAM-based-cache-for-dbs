
#ifndef _GLIBCXX_STRING
#include <string>
#endif

#ifndef CORE_H 
#define CORE_H
namespace core {
    enum TYPES {
        INT,
        FLOAT,
        STRING,
    };

    class Record {
        TYPES t;
        int ttl;

        protected:

        Record(TYPES );
    };

    class Int: public Record {
        int data;
        int get();
        void set(int );

        public:

        Int(int );
        int test();
    };

    class Float: public Record {
        float data;
        float get();
        void set(float );

        public:

        Float(float );
        float test();
    };

    class String: public Record {
        std::string data;
        std::string get();
        void set(std::string );

        public:

        String(std::string );
        std::string test();
    };

}
#endif
