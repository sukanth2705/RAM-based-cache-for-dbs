#include "cache/server.h"

int main()
{
    core::Cache c;
    core::Record<int> v1(3), v2(4), v3(5);
    c.set("a", &v1);
    c.set("b", &v2);
    c.set("c", &v3);
    initialize(&c);
}