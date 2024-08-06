#include "cache/core.h"
#include <iostream>
using namespace core;
int main()
{
    Record<int> a(1);
    Cache c;
    c.set("abcd", &a);
    Record<int> *ptr = dynamic_cast<Record<int> *>(c.get("abcd"));
    std::cout << ptr->getTypeName() << std::endl;
    std::cout << ptr->get() << std::endl;
    return 0;
}