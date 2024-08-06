#include "cache/server.h"
#include <iostream>
#include <thread>

int main()
{
    Cache c(0);
    Record<int> v1(3), v2(4), v3(5);
    c.set("a", &v1);
    c.set("b", &v2);
    c.set("c", &v3);
    initialize(&c);
}