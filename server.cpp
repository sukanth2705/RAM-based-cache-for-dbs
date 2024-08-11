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
    while (!is_carshed)
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
    char buff[1024] = "-OK\r\n";
    decode(buff);
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        std::cout << "Socket creation failed" << std::endl;
        is_carshed = true;
        return;
    }

    if (set_non_blocking(server_sock) < 0)
    {
        std::cout << "Error in setting socket to non blocking mode" << std::endl;
        is_carshed = true;
        return;
    }

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "Unable to bind socket" << std::endl;
        is_carshed = true;
        return;
    }
    if (listen(server_sock, 5) < 0)
    {
        std::cout << "Unable to listen for connections" << std::endl;
        is_carshed = true;
        return;
    }

    fd_set readfds;
    int max_fd = server_sock;
    std::vector<int> client_sock;
    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(server_sock, &readfds);
        for (int i = 0; i < client_sock.size(); i++)
        {
            FD_SET(client_sock[i], &readfds);
            max_fd = std::max(max_fd, client_sock[i]);
        }
        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        std::cout<<"hello"<<std::endl;
        if (activity < 0)
        {
            std::cout << "Error in select call" << std::endl;
            is_carshed = true;
            return;
        }
        socklen_t len = sizeof(serv_addr);
        if (FD_ISSET(server_sock, &readfds))
        {
            int sock = accept(server_sock, (sockaddr *)&serv_addr, &len);
            if (sock < 0)
            {
                std::cout << "Error accepting connection" << std::endl;
            }
            else if (set_non_blocking(sock) < 0)
            {
                std::cout << "Error in setting socket to non blocking mode" << std::endl;
            }
            else
            {
                client_sock.push_back(sock);
            }
        }
        for (int i = 0; i < client_sock.size(); i++)
        {
            if (FD_ISSET(client_sock[i], &readfds))
            {
            }
        }
    }
}

void initialize(Cache *db)
{
    std::thread master_th(master, db);
    std::thread cleaner_th(cleaner, db);
    master_th.join();
    cleaner_th.join();
}
