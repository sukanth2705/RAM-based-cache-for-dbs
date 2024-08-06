#ifndef SERVER_H
#define SERVER_H

#include "core.h"

void persistance(Cache *);
void cleaner();
void master();
void initialize(Cache *);

#endif