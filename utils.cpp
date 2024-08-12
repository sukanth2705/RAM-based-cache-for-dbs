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

int set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, O_NONBLOCK | flags);
}

void encode(char *encoded_value, std::vector<std::string> input)
{
    memset(encoded_value, '\0', sizeof(encoded_value));
    int val_index = 0;
    for (int i = 0; i < input.size(); i++)
    {
        for (int j = 0; j < input[i].size(); j++)
        {
            encoded_value[val_index++] = input[i][j];
        }
        encoded_value[val_index++] = '\r';
        encoded_value[val_index++] = '\n';
    }
}

void handle_commands(int command, char *token, char *msg, Cache *db)
{
    COMMANDS cmd = static_cast<COMMANDS>(command);
    if (cmd == SET)
    {
        std::cout << "set" << std::endl;

    }
    else if (cmd == GET)
    {
        std::cout << "get" << std::endl;
        token = strtok(NULL,"\r\n");
        BaseRecord *answer;
        answer = db->get(token);
        TYPE anstyp = answer->type;
        if (anstyp == STRING)
        {
            Record<std::string> *data;
            data =dynamic_cast<Record<std::string> *>(answer);
            if(data == nullptr)
            {
                encode(msg, {"-1"});
                return;
            }
            else
            {
                std::string val = data->get();
                std::string message = "+" + val;
                encode(msg,{message});
                return;
            }

        }
        else if (anstyp == FLOAT)
        {
            Record<float> *data;
            data = dynamic_cast<Record<float> *>(answer);
            if(data == nullptr)
            {
                encode(msg, {"-1"});
                return;
            }
            else
            {
                float val = data->get();
                std::string message = "+" + std::to_string(val);
                encode(msg,{message});
                return;
            }
        }
        else if (anstyp == INT)
        {
            Record<int> *data;
            data = dynamic_cast<Record<int> *>(answer);
            if(data == nullptr)
            {
                encode(msg, {"-1"});
                return;
            }
            else
            {
                int val = data->get();
                std::string message = "+" + std::to_string(val);
                encode(msg,{message});
                return;
            }
        }
    }
    else if (cmd == PING)
    {
        encode(msg, {"+PONG"});
    }
}

void decode(char *msg,Cache* db)
{
    char buff[1024];
    memset(buff, '\0', sizeof(buff));
    strcpy(buff, msg);
    char *token = strtok(buff, "\r\n");
    if (buff[0] == '+' || buff[0] == '-')
    {
        std::cout << token + 1 << std::endl;
        return;
    }
    handle_commands(atoi(token), token, msg, db);
}
