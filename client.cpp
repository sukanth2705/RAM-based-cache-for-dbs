#include "cache/client.h"
#include <string>
#include <vector>

void Client::encoder(std::string input)
{
    int i = 1,inputSize = input.size();
    memset(encodedValue,'\0',1024);
    Client::encodedValue[0] = '+';
    while(i <= inputSize)
    {
        Client::encodedValue[i] = input[i-1];
        i++;
    }
    Client::encodedValue[i++] = '\\';
    Client::encodedValue[i++] = 'r';
    Client::encodedValue[i++] = '\\';
    Client::encodedValue[i++] = 'n';
    return;
}

