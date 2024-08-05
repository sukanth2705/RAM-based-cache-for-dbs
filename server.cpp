#include "core/core.h"
#include <thread>
#include <fstream>
#include <iostream>
#include <string>
#include <map>

int opCount = 0;
int lock = 0;
std::string operation;
std::string logErrorMessage = "Unable to open log files";
int logFlush = 100;
core::Cache c;
  
void persistance(){
    while (lock!=1){
        continue;
    }
    
    if (opCount <= logFlush){
        std::ofstream differentialLog;
        differentialLog.open("logs\\differentiallogs.txt",std::ios::app);
        if (differentialLog){
            differentialLog<<opCount<<","<<operation<<'\n';
            differentialLog.close();
        }
        else {
            std::cerr<<logErrorMessage;
        }
        differentialLog.close();
    }
    else{
        std::ofstream differentialLog;
        differentialLog.open("logs\\differentiallogs.txt");
        if (differentialLog){
            differentialLog.close();
            std::ofstream tableCaptureLog;
            tableCaptureLog.open("logs\\tablecapturelog.txt");
            if (tableCaptureLog){
                for(auto it=c.table.begin(); it != c.table.end(); it++ ){
                    tableCaptureLog<<it->first<<' '<<it->second<<'\n';
                }
            }
            else{
                std::cerr<<logErrorMessage;
            }
            tableCaptureLog.close();
        }
        else{
            std::cerr<<logErrorMessage;
        }
        differentialLog.close();
    }
    
    lock=0;  
}

void cleaner(){
    while (lock!=2) {
        continue;
    }

    

}
    

void master(){


}

int main(){
    core::Cache c;

}