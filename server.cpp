#include "cache/server.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <thread>

int opCount = 0;
int lock = 1;
std::string operation = "get";
std::string logErrorMessage = "Unable to open log files"; 
int logFlush = 2;

bool fileChecker(std::ofstream &fileObject) //function to check if file object is opened properly
{
    if (fileObject) return true;
    else
    {
        std::cerr << logErrorMessage;
        fileObject.close();
        return false;
    }
}

void persistance(Cache* c)
{
    std::cout<<"hello3\n";
    std::ofstream differentialLog, tableCaptureLog;
    if (opCount+1 <= logFlush)
    {
        differentialLog.open("../../logs/differentiallogs.txt", std::ios::app);//opening differentiallogs.txt to append command
        fileChecker(differentialLog);
        differentialLog << opCount << "," << operation << '\n';
        opCount++;
        differentialLog.close();
    }
    else
    {
        differentialLog.open("../../logs/differentiallogs.txt"); //the following 3 lines clear differentiallogs.txt by opening in write mode and immediately closing
        fileChecker(differentialLog);
        differentialLog.close();
        tableCaptureLog.open("../../logs/tablecapturelog.txt");//opening tablecapturelog.txt and then copying table
        fileChecker(tableCaptureLog);
        for (auto it = (*c).table.begin(); it != (*c).table.end(); it++)
        {
            tableCaptureLog << it->first << ' ' << it->second << '\n';
        }
        opCount = 0;
        tableCaptureLog.close();
    }
}
    
    

void cleaner()
{
    std::cout<<"hello2\n";
    while (lock == 2)
    {
        continue;
    }
}

void master()
{
    std::cout<<"hello1\n";
    return ;
}

void initialize(Cache* c){
    std::thread master_th(master);
    std::thread persistance_th(persistance,c);
    std::thread cleaner_th(cleaner);
    master_th.join();
    persistance_th.join();
    cleaner_th.join();
}
