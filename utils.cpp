#include "cache/utils.h"

#include <algorithm>
#include <fcntl.h>
#include <fstream>
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
    if (db->num_keys == 0)
    {
        return sample;
    }
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

void persistance(Cache *db, std::string operated_key)
{
    std::string absolute_log_file_path = std::string(FLAGS_log_path) + "/" + std::string(FLAGS_log_file);
    std::ofstream log;
    log.open(absolute_log_file_path, std::ios::app);
    Record<int> *operated_record = dynamic_cast<Record<int> *>(db->get(operated_key));
    int time_elapsed = get_elapsed_seconds(operated_record);
    if (log && time_elapsed < operated_record->ttl)
    {
        log << operated_record->type << " " << operated_key << " " << operated_record->get() << " " << operated_record->ttl - time_elapsed << std::endl;
    }
    else
    {
        std::cout << "Unable to record this operation in log file.\n";
    }
    log.close();
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
        std::string key, type_str, value, ttl;
        key = strtok(NULL, "\r\n");
        type_str = strtok(NULL, "\r\n");
        ttl = strtok(NULL, "\r\n");
        value = strtok(NULL, "\r\n");
        int TTL = std::stoi(ttl);
        int t = std::stoi(type_str);
        TYPE type = static_cast<TYPE>(t);
        if (type == STRING)
        {
            std::string val = value;
            Record<std::string> *data = new Record<std::string>(val, TTL);
            db->set(key, data);
        }
        else if (type == FLOAT)
        {
            float val = std::stof(value);
            Record<float> *data = new Record<float>(val, TTL);
            db->set(key, data);
        }
        else if (type == INT)
        {
            int val = std::stoi(value);
            Record<int> *data = new Record<int>(val, TTL);
            db->set(key, data);
        }
        persistance(db, key);
        encode(msg, {"+OK"});
    }
    else if (cmd == GET)
    {
        std::cout << "get" << std::endl;
        token = strtok(NULL, "\r\n");
        BaseRecord *answer;
        answer = db->get(token);
        TYPE anstyp = answer->type;
        if (anstyp == STRING)
        {
            Record<std::string> *data;
            data = dynamic_cast<Record<std::string> *>(answer);
            if (data == nullptr)
            {
                encode(msg, {"-1"});
            }
            else
            {
                std::string val = data->get();
                std::string message = "+" + val;
                encode(msg, {message});
            }
        }
        else if (anstyp == FLOAT)
        {
            Record<float> *data;
            data = dynamic_cast<Record<float> *>(answer);
            if (data == nullptr)
            {
                encode(msg, {"-1"});
            }
            else
            {
                float val = data->get();
                std::string message = "+" + std::to_string(val);
                encode(msg, {message});
            }
        }
        else if (anstyp == INT)
        {
            Record<int> *data;
            data = dynamic_cast<Record<int> *>(answer);
            if (data == nullptr)
            {
                encode(msg, {"-1"});
            }
            else
            {
                int val = data->get();
                std::string message = "+" + std::to_string(val);
                encode(msg, {message});
            }
        }
    }
    else if (cmd == PING)
    {
        encode(msg, {"+PONG"});
    }
}

bool decode(char *msg, Cache *db)
{
    char buff[1024];
    memset(buff, '\0', sizeof(buff));
    strcpy(buff, msg);
    char *token = strtok(buff, "\r\n");
    if (buff[0] == '+')
    {
        std::cout << token + 1 << std::endl;
        return true;
    }
    else if (buff[0] == '-')
    {
        std::cout << token + 1 << std::endl;
        return false;
    }
    handle_commands(atoi(token), token, msg, db);
    return true;
}
