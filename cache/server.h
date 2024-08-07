#ifndef SERVER_H
#define SERVER_H

#include "core.h"
#include "flags.h"
#include <vector>

void persistance(Cache *);
Cache *reconstructor(int &errorcode);
void cleaner(Cache *);
void master(Cache *);
void initialize(Cache *);
std::vector<std::string> randomSample(Cache* );

#endif