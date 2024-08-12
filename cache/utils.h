#ifndef UTILS_H
#define UTILS_H

#include "core.h"
#include "flags.h"

#include <string>
#include <vector>

int get_elapsed_seconds(Record<int> *);
std::vector<std::string> random_sample(Cache *);
void encode(char *encodedValue, std::vector<std::string> input);
int set_non_blocking(int);
void persistance(Cache *, std::string);
bool decode(char *, Cache *);

#endif
