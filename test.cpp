#include "cache/client.h"

#include <iostream>

int main()
{
    Client api("127.0.0.1", 8080);
    api.set("key", INT, "5", 10000);
    std::cout << "hello\n";
    std::cout << api.encoded_value << std::endl;
    api.get("key");
    std::cout << api.encoded_value << std::endl;
    return 0;
}
