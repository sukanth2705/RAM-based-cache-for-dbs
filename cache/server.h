#ifndef SERVER_H
#define SERVER_H

#include "core.h"

void persistance(core::Cache *);
void cleaner();
void master();
void initialize(core::Cache *);

#endif