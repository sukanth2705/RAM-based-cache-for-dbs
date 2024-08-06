#include "cache/server.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <thread>

int opCount = 0;
int lock = 1;
std::string operation = "get";
std::string logErrorMessage = "Unable to open log files";
int logFlush = 2;

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

void persistance(Cache *c)
{
    std::ofstream differentialLog, tableCaptureLog;
    differentialLog.open("../../logs/differentiallogs.txt", std::ios::app);
    tableCaptureLog.open("../../logs/tablecapturelog.txt");
    if (!differentialLog)
    {
        std::cerr << logErrorMessage;
        differentialLog.close();
        return;
    }
    if (!tableCaptureLog)
    {
        std::cerr << logErrorMessage;
        tableCaptureLog.close();
        return;
    }
    while (1)
    {
        if (opCount + 1 <= logFlush)
        {
            differentialLog << opCount << "," << operation << '\n';
            opCount++;
        }
        else
        {
            for (auto it = (*c).table.begin(); it != (*c).table.end(); it++)
            {
                tableCaptureLog << it->first << ' ' << it->second << '\n';
            }
            opCount = 0;
            break;
        }
    }
    differentialLog.close();
    tableCaptureLog.close();
}

void cleaner(Cache *c)
{
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
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
