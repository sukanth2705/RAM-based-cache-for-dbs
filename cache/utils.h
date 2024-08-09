#ifndef UTILS_H
#define UTILS_H

#include "core.h"
#include "flags.h"

#include <string>
#include <vector>

int get_elapsed_seconds(Record<int> *);
std::vector<std::string> randomSample(Cache *);

#endif
