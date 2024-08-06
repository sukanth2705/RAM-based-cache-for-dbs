#include "cache/core.h"
#include "cache/server.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>

int opCount = 0;
int lock = 1;
std::string operation = "get";
std::string logErrorMessage = "Unable to open log files";
int logFlush = 2;
core::Cache c;

void persistance()
{
    std::ofstream differentialLog, tableCaptureLog;
    differentialLog.open("../../logs/differentiallogs.txt", std::ios::app);
    tableCaptureLog.open("../../logs/tablecapturelog.txt");
    if (!differentialLog)
    {
        std::cerr << logErrorMessage;
        differentialLog.close();
        return;
    }
    if (!tableCaptureLog)
    {
        std::cerr << logErrorMessage;
        tableCaptureLog.close();
        return;
    }
    while (1)
    {
        if (opCount+1 <= logFlush)
        {
            differentialLog << opCount << "," << operation << '\n';
            opCount++;
        }
        else
        {
            for (auto it = c.table.begin(); it != c.table.end(); it++)
            {
                tableCaptureLog << it->first << ' ' << it->second << '\n';
            }
            opCount = 0;
            break;
        }
    }
    differentialLog.close();
    tableCaptureLog.close();
}

void cleaner()
{
    while (lock != 2)
    {
        continue;
    }
}

void master()
{
    return ;
}


