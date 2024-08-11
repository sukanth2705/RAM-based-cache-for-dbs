#include "cache/utils.h"
#include "cache/client.h"

#include <algorithm>
#include <random>

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

char *encode(char *encodedValue, std::string input)
{
    encodedValue[0] = '+';
    std::string command;
    int i=0;

    for(i = 0; i < 3; i++)
    {
        command += input[i]; 
    }
    if (command == "SET")
    {
        encodedValue[1] = '1';
    }
    else if (command == "GET")
    {
        encodedValue[1] = '0';
    }
    else if (command == "PIN")
    {
        encodedValue[1] = '2';    
    }
    
    encodedValue = seperator(encodedValue,2);

    

    return encodedValue;
}


