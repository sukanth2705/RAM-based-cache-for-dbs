#ifndef CLIENT_H
#define CLIENT_H

#include "core.h"
#include "flags.h"
#include "utils.h"
#include "server.h"

#include <vector>


bool send();
std::string recieve();
void open();
void close();


#endif