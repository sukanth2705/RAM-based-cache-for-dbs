
#include "cache/core.h"
#include "cache/server.h"
#include "cache/utils.h"
#include "cache/flags.h"
#include <string.h>
#include <iostream>
#include "cache/client.h"

int main(int argc,char * argv[])
{
    Client client = Client::Client("127.0.01",8050);
    std::string command;
    command = argv[0];
    int returnval;
    if (command == "SET")
    {
        int cmd = static_cast<int>(SET);
        command = std::to_string(cmd);
        TYPE typ;
        std::string key,type,value,ttl;
        key = argv[1];
        type = argv[2];
        value = argv[3];
        if (argc == 5)
        {
            ttl = argv[4];
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
            std::cout<<"Invalid data type"<<std::endl;
            returnval = -2;
        }
        client.set(key,typ,ttl);
    }
    else if (command == "GET")
    {
        std::string key = argv[1];
        client.get(key);
        returnval = 0;
    }
    else if (command == "PING")
    {
        client.ping();
    }
    else 
    {
        std::cout<<"Invalid command";
        returnval = -1;
    }
    client.close_connection();
    return  returnval;

}