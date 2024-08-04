#include "core.h"

#include<iostream>

core::Int::Int(int data):data(data){}
int core::Int::get() {return this->data;}
void core::Int::set(int val) {this->data=val;}
int core::Int::test(){return this->data;}

int main(){
    core::Int v(4);
    std::cout<<v.test()<<std::endl;
    return 0;
}