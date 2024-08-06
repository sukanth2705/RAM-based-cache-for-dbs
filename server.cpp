#include "core/core.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>

int opCount = 0;
int lock = 1;
std::string operation="get";
std::string logErrorMessage = "Unable to open log files";
int logFlush = 2;
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
        }
        else {
            std::cerr<<logErrorMessage;
        }
        differentialLog.close();
        opCount++;
    }
    else{
        std::ofstream differentialLog;
        differentialLog.open("logs\\differentiallogs.txt");
        if (differentialLog){
            differentialLog.close();
            std::ofstream tableCaptureLog;
            tableCaptureLog.open("logs\\tablecapturelog.txt");
            std::cout<<"re1";
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
        opCount=0;
    }
     
}

void cleaner(){
    while (lock!=2) {
        continue;
    }

    

}
    

void master(){


}

int main(){
    core::Record<int> v1(3),v2(4),v3(5);
    c.set("a",&v1);
    c.set("b",&v2);
    c.set("c",&v3);
    persistance();
    persistance();
    persistance();
    persistance();
}