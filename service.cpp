#include "cache/server.h"

#include <iostream>
#include <thread>

int main()
{
    Cache db;
    initialize(&db);
    std::cout << "Service initialized and started .....\n";
}
