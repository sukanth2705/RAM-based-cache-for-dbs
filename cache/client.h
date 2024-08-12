#ifndef CLIENT_H
#define CLIENT_H

#include "core.h"
#include "flags.h"
#include "utils.h"

#include <netinet/in.h>
#include <string>
#include <vector>

class Client
{
    int client_sock;
    sockaddr_in serv_addr;

public:
    char encoded_value[1024];
    Client(std::string, int);
    bool ping();
    bool get(std::string);
    bool set(std::string, TYPE, std::string);
    bool set(std::string, TYPE, std::string, int);
    void close_connection();
};

#endif
