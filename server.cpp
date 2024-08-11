#include "cache/server.h"

#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

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

void cleaner(Cache *db)
{
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(FLAGS_cleaner_timeout));
        std::vector<std::string> sample_keys = random_sample(db);
        std::chrono::time_point<std::chrono::system_clock> currenttimestamp = std::chrono::system_clock::now();
        for (int i = 0; i < sample_keys.size(); i++)
        {
            const auto record = dynamic_cast<Record<int> *>(db->table[sample_keys[i]]);
            if (get_elapsed_seconds(record) >= record->ttl)
            {
                db->table.erase(sample_keys[i]);
                db->num_keys--;
                db->keys.erase(sample_keys[i]);
            }
        }
    }
}

void master(Cache *db)
{
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        std::cout << "Socket creation failed" << std::endl;
        return;
    }

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "Unable to bind socket" << std::endl;
        return;
    }
    if (listen(server_sock, 5))
    {
        std::cout << "Unable to listen for connections" << std::endl;
    }
    while (1)
    {
        std::cout << "master:";
        std::cout << db->num_keys << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void initialize(Cache *db)
{
    std::thread master_th(master, db);
    std::thread cleaner_th(cleaner, db);
    master_th.join();
    cleaner_th.join();
}
