#include "core.h"
#include<iostream>

namespace core{
    Record::Record(TYPES t):t(t){}

    Int::Int(int data):data(data),Record(INT){}
    int Int::get() {return this->data;}
    void Int::set(int val) {this->data=val;}
    int Int::test(){return this->data;}

    Float::Float(float data):data(data),Record(FLOAT){}
    float Float::get() {return this->data;}
    void Float::set(float val) {this->data=val;}
    float Float::test(){return this->data;}

    String::String(std::string data):data(data),Record(STRING){}
    std::string String::get() {return this->data;}
    void String::set(std::string val) {this->data=val;}
    std::string String::test(){return this->data;}
}

int main(){
    core::Int v(4);
    core::Float f(3.5);
    core::String s("abcd");
    std::cout<<v.test()<<std::endl;
    std::cout<<f.test()<<std::endl;
    std::cout<<s.test()<<std::endl;
    return 0;
}