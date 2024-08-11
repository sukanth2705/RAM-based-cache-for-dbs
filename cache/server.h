#ifndef SERVER_H
#define SERVER_H

#include "core.h"
#include "flags.h"
#include "utils.h"

#include <string>
#include <vector>

void persistance(Cache *, std::string);
void cleaner(Cache *);
void master(Cache *);
void initialize(Cache *);

bool is_carshed = false;

#endif
