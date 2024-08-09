#include "cache/server.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <thread>

std::vector<std::string> randomSample(Cache *c)
{
    std::vector<std::string> sample;
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

bool readFileChecker(std::ifstream &fileObject) // function to check if file object is opened properly
{
    if (fileObject)
        return true;
    else
    {
        std::cerr << "Unable to reconstruct Cache as log files are not opening";
        fileObject.close();
        return false;
    }
}

void persistance(Cache *c, std::string operated_key)
{
    std::string absolute_log_file_path = std::string(FLAGS_log_path) + "/" + std::string(FLAGS_log_file);
    std::cout << absolute_log_file_path << std::endl;
    std::ofstream log;
    log.open(absolute_log_file_path, std::ios::app); // opening log file to append command
    Record<int> *operated_record = dynamic_cast<Record<int> *>(c->get(operated_key));
    if (log)
    {
        log << operated_key << " " << operated_record->get() << std::endl;
    }
    else
    {
        std::cout << "Unable to record this operation in log file.\n";
    }
    log.close();
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

void master(Cache *c)
{
    Record<int> v1(3, 10), v2(4), v3(5);
    c->set("a", &v1);
    persistance(c, "a");
    c->set("b", &v2);
    persistance(c, "b");
    c->set("c", &v3);
    persistance(c, "c");
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
    std::thread cleaner_th(cleaner, c);
    master_th.join();
    cleaner_th.join();
}
