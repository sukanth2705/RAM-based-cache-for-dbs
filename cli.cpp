#include "cache/client.h"

#include <iostream>
#include <string.h>

int main(int argc, char *argv[])
{
    Client client("127.0.0.1", 8080);
    std::string command;
    command = argv[1];
    if (command == "SET")
    {
        int cmd = static_cast<int>(SET);
        command = std::to_string(cmd);
        TYPE typ;
        std::string key, type, value, ttl;
        key = argv[2];
        type = argv[3];
        value = argv[4];
        if (argc == 6)
        {
            ttl = argv[5];
        }
        else
        {
            ttl = FLAGS_ttl;
        }
        if (type == "float")
        {
            typ = FLOAT;
        }
        else if (type == "string")
        {
            typ = STRING;
        }
        else if (type == "int")
        {
            typ = INT;
        }
        else
        {
            std::cout << "Invalid data type\n";
            exit(EXIT_FAILURE);
        }
        client.set(key, typ, ttl);
    }
    else if (command == "GET")
    {
        std::string key = argv[2];
        client.get(key);
    }
    else if (command == "PING")
    {
        client.ping();
    }
    else
    {
        std::cout << "Invalid command\n";
        exit(EXIT_FAILURE);
    }
    std::cout << client.encoded_value << std::endl;
    client.close_connection();
    exit(EXIT_SUCCESS);
}