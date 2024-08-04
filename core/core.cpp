#include "core.h"
#include<iostream>

namespace core{
    Int::Int(int data):data(data){}
    int Int::get() {return this->data;}
    void Int::set(int val) {this->data=val;}
    int Int::test(){return this->data;}

    Float::Float(float data):data(data){}
    float Float::get() {return this->data;}
    void Float::set(float val) {this->data=val;}
    float Float::test(){return this->data;}
}

int main(){
    core::Int v(4);
    core::Float f(3.5);
    std::cout<<v.test()<<std::endl;
    std::cout<<f.test()<<std::endl;
    return 0;
}