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

bool writeFileChecker(std::ofstream &fileObject) //function to check if file object is opened properly
{
    if (fileObject) return true;
    else
    {
        std::cerr <<"Unable to open log file";
        fileObject.close();
        return false;
    }
}

bool readFileChecker(std::ifstream &fileObject) //function to check if file object is opened properly
{
    if (fileObject) return true;
    else
    {
        std::cerr <<"Unable to reconstruct Cache as log files are not opening";
        fileObject.close();
        return false;
    }
}

void persistance(Cache* c)
{
    std::ofstream logPath;
    logPath.open(FLAGS_log_path, std::ios::app);//opening differentiallogs.txt to append command
    writeFileChecker(logPath);
    logPath << opCount << "," << operation << '\n';
    opCount++;
    logPath.close();
    opCount++;
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

Cache *reconstructor(int &errorcode)
{
    Cache *c;
    std::string line,key = "";
    std::ifstream logPath;
    int data,i;
    bool flag = false;
    logPath.open(FLAGS_log_path);
    while(std::getline(logPath,line))
    {
        for( i = 0 ; i < line.size() ;i++)
        {
            if (line[i]==',') 
            {
                flag = true;
                continue;
            }
            if (flag)
            {
                data = data*10 + (line[i]-'0');
            }
            else
            {
                key += line[i];
            }
        }
        Record <int> v1(data);
        (*c).set(key, &v1);
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
