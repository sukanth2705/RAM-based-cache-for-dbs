#include "cache/server.h"
#include "cache/flags.h"
#include "cache/core.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <thread>

int opCount = 0;
std::string operation;

std::vector<std::string> randomSample(Cache *c)
{
    std::vector<std::string> sample;
    if (c->num_keys <= 0)
    {
        throw std::invalid_argument("Sample size cannot be larger than population size");
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, c->num_keys - 1);
    std::vector<int> indices(c->num_keys);
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), gen);
    for (auto it = c->keys.begin(); it != c->keys.end(); ++it)
    {
        sample.push_back(*it);
    }

    return sample;
}

bool fileChecker(std::ofstream &fileObject) //function to check if file object is opened properly
{
    if (fileObject) return true;
    else
    {
        std::cerr <<"Unable to open log file";
        fileObject.close();
        return false;
    }
}

void persistance(Cache* c)
{
    std::ofstream differentialLog, tableCaptureLog;
    if (opCount+1 <= FLAGS_operations_before_tablecapture)
    {
        differentialLog.open(FLAGS_differentiallog_path, std::ios::app);//opening differentiallogs.txt to append command
        fileChecker(differentialLog);
        differentialLog << opCount << "," << operation << '\n';
        opCount++;
        differentialLog.close();
    }
    else
    {
        differentialLog.open(FLAGS_differentiallog_path); //the following 3 lines clear differential.log by opening in write mode and immediately closing
        fileChecker(differentialLog);
        differentialLog.close();
        tableCaptureLog.open(FLAGS_tablecapturelog_path);//opening tablecapture.log and then copying table
        fileChecker(tableCaptureLog);
        for (auto it = (*c).table.begin(); it != (*c).table.end(); it++)
        {
            tableCaptureLog << it->first << ' ' << it->second << '\n';
        }
        opCount = 0;
        tableCaptureLog.close();
    }
}
    
    

void cleaner(Cache *c)
{
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(FLAGS_cleaner_timeout));
        std::vector<std::string> sample_keys = randomSample(c);
        std::chrono::time_point<std::chrono::system_clock> currenttimestamp = std::chrono::system_clock::now();
        for (int i = 0; i < sample_keys.size(); i++)
        {
            const auto record = dynamic_cast<Record<int> *>(c->table[sample_keys[i]]);
            auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(currenttimestamp - record->timestamp);
            std::cout << elapsed_seconds.count() << std::endl;
            if (elapsed_seconds.count() > record->ttl)
            {
                c->table.erase(sample_keys[i]);
                c->num_keys--;
                c->keys.erase(sample_keys[i]);
            }
        }
    }
}

void reconstructor(int &errorcode){

}

void master(Cache *c)
{
    while (1)
    {
        std::cout << "master:";
        std::cout << c->num_keys << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void initialize(Cache *c)
{
    std::thread master_th(master, c);
    std::thread persistance_th(persistance, c);
    std::thread cleaner_th(cleaner, c);
    master_th.join();
    persistance_th.join();
    cleaner_th.join();
}
