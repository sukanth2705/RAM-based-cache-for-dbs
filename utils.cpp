#include "cache/utils.h"
#include "cache/client.h"

#include <algorithm>
#include <fcntl.h>
#include <iostream>
#include <random>
#include <string.h>
#include <sstream>
#include <vector>

int get_elapsed_seconds(Record<int> *record)
{
    std::chrono::time_point<std::chrono::system_clock> currenttimestamp = std::chrono::system_clock::now();
    auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(currenttimestamp - record->timestamp);
    return elapsed_seconds.count();
}


std::vector<std::string> random_sample(Cache *db)
{
    std::vector<std::string> sample;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, db->num_keys - 1);
    std::vector<int> indices(db->num_keys);
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), gen);
    for (auto it = db->keys.begin(); it != db->keys.end(); ++it)
    {
        sample.push_back(*it);
    }
    return sample;
}


char *seperator(char *encodedValue,int index)
{
    encodedValue[index++] = '\\';
    encodedValue[index++] = 'r';
    encodedValue[index++] = '\\';
    encodedValue[index++] = 'n';
    return encodedValue;
}

char *copier(char *encodedValue,std::string input,int valIndex){
    for(int i = 0; i < input.size(); i++){
        encodedValue[valIndex++]=input[i];
    }
    
    encodedValue = seperator(encodedValue,valIndex);
    valIndex += 4;
    return encodedValue;
}

char *encode(char *encodedValue, std::vector<std::string> input)
{
    encodedValue[0] = '+';
    int valIndex = 1;
    for(int i = 0; i++; i<4)
    {
        encodedValue = copier(encodedValue,input[i],valIndex);
        valIndex = input[i].size()+4;
    }
    if (input.size() == 5) 
    {
        encodedValue = copier(encodedValue,input[4],valIndex);
        valIndex = input[4].size()+4;
    }
    return encodedValue;
}

int set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, O_NONBLOCK | flags);
}

void handle_commands(int command, char *token)
{
    COMMANDS cmd = static_cast<COMMANDS>(command);
    if (cmd == SET)
    {
        std::cout << "set" << std::endl;
    }
    else if (cmd == GET)
    {
        std::cout << "get" << std::endl;
    }
    else if (cmd == PING)
    {
        std::cout << "ping" << std::endl;
    }
}

void decode(const char *msg)
{
    char buff[1024];
    memset(buff, '\0', sizeof(buff));
    strcpy(buff, msg);
    char *token = strtok(buff, "\r\n");
    if (buff[0] == '+' || buff[0] == '-')
    {
        std::cout << token + 1 << std::endl;
        return ;
    }
    handle_commands(atoi(token), token);
}
