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
    char encoded_value[1024];
    int client_sock;
    sockaddr_in serv_addr;

public:
    Client(std::string, int);
    void ping();
    void get(std::string);
    void set(std::string, TYPE, std::string);
    void set(std::string, TYPE, std::string, int);
    void close_connection();
};

#endif