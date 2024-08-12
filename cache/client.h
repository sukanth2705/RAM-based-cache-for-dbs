#ifndef CLIENT_H
#define CLIENT_H

#include "core.h"
#include "flags.h"
#include "utils.h"

#include <vector>

class Client {
    public :
        char encodedValue[1024];
        bool send();
        std::string receive();
        void open();
        void close();
        void encoder(std::string input);
};

#endif