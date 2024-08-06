#ifndef SERVER_H
#define SERVER_H

#include "core.h"
#include <vector>

void persistance(Cache *);
void cleaner(Cache *);
void master(Cache *);
void initialize(Cache *);
std::vector<std::string> randomSample(Cache* );

#endif