//
// Created by Ivan on 11/8/2018.
//
#pragma once

//Structs
struct set1{
    uint32_t a;
};

struct set3{
    string first;
    string last;
};

struct set4{
    uint8_t a;
    uint16_t b;
    uint32_t c;
    uint64_t d;
    int8_t e;
    int16_t f;
    int32_t g;
    int64_t h;
    float i;
    double j;
}__attribute__((packed));

//Output
void dump1(set1 temp){
    cout<<(int)temp.a<<endl;
}

void dump3(set3 temp){
    cout<<"Dumping set3:"<<endl;
    cout<<temp.first<<endl;
    cout<<temp.last<<endl;
}

void dump4(set4 temp){
    cout<<(int)temp.a<<endl;
    cout<<temp.b<<endl;
    cout<<temp.c<<endl;
    cout<<temp.d<<endl;
    cout<<(int)temp.e<<endl;
    cout<<temp.f<<endl;
    cout<<temp.g<<endl;
    cout<<temp.h<<endl;
    cout<<temp.i<<endl;
    cout<<temp.j<<endl;
}