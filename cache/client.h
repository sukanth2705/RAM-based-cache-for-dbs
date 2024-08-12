#ifndef CLIENT_H
#define CLIENT_H

#include "core.h"
#include "flags.h"
#include "utils.h"

#include <vector>

class Client
{
    char encodedValue[1024];

public:
    bool send();
    void receive();
    void open();
    void close();
};

#endif