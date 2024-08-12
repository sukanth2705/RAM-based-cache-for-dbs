#include "cache/client.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

Client::Client(std::string server_ip, int port)
{
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr);
    connect(client_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
}

bool Client::ping()
{
    encode(encoded_value, {std::to_string(static_cast<int>(PING))});
    send(client_sock, encoded_value, sizeof(encoded_value), 0);
    memset(encoded_value,'\0',sizeof(encoded_value));
    recv(client_sock,encoded_value,sizeof(encoded_value),0);
    if(encoded_value[0]=='+'){
        return true;
    }
    return false;
}

bool Client::get(std::string key)
{
    encode(encoded_value, {std::to_string(static_cast<int>(GET)), key});
    send(client_sock, encoded_value, sizeof(encoded_value), 0);
    memset(encoded_value,'\0',sizeof(encoded_value));
    recv(client_sock,encoded_value,sizeof(encoded_value),0);
    if(encoded_value[0]=='+'){
        return true;
    }
    return false;
}

bool Client::set(std::string key, TYPE type, std::string value, int ttl)
{
    encode(encoded_value, {std::to_string(static_cast<int>(SET)), key, std::to_string(static_cast<int>(type)), value, std::to_string(ttl)});
    send(client_sock, encoded_value, sizeof(encoded_value), 0);
    memset(encoded_value,'\0',sizeof(encoded_value));
    recv(client_sock,encoded_value,sizeof(encoded_value),0);
    if(encoded_value[0]=='+'){
        return true;
    }
    return false;
}

bool Client::set(std::string key, TYPE type, std::string value)
{
    return set(key, type, value, FLAGS_ttl);
}

void Client::close_connection()
{
    close(client_sock);
}
