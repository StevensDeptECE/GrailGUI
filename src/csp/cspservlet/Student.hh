//
// Created by Ivan on 11/26/2018.
//

#pragma once
#include <string>
#include <cstring>
#include <iostream>

class Student{
private:
    std::string first;
    std::string last;
    uint32_t id;
public:
    Student(const std::string& first, const std::string& last, uint16_t id) : first(first), last(last), id(id) {}
    Student(){
        first="fdefault";
        last="ldefault";
        id=0;
    }

    uint32_t serializeSize() const { return sizeof(uint32_t)*3+first.length()+last.length(); }

    std::string const getFirst(){
        return first;
    }

    std::string const getLast(){
        return last;
    }

    uint32_t const getID(){
        return id;
    }

    char * write(char *p){
        //Value of ID
        *(uint32_t*)p=id;
        p+=sizeof(uint32_t);
        //Length of First
        *(uint32_t*)p =first.length();
        p+=sizeof(uint32_t);
        //Length of Last
        *(uint32_t*)p =last.length();
        p+=sizeof(uint32_t);
        //Value of First
        //memcpy(p,&first,first.length());
        //p+=first.length();
        for (uint32_t i = 0;i<first.length();i++){
            *p=first.at(i);
            ++p;
        }
        //Value of Last
       // memcpy(p,&last,last.length());
        for (uint32_t i = 0;i<last.length();i++){
            *p=last.at(i);
            ++p;
        }
        return p;
    }
    //Student(const Student &cSource)
//    Student(const Student&  src) : first(src.first), last(src.last), id(src.id) {}
/*
    //TODO:Gets here and segfaults; sdt has all values correct; "this" has the following error
    //TODO:<error reading variable: Cannot access memory at address 0x433b6e69625c7451>{static npos = 18446744073709551615, _M_dataplus = {<std::allocator<char>> = {<__gnu_cxx::new_allocator<char>> = {<No data fields>}, <No data fields>}, _M_p = 0x433b6e69625c7469 <error: Cannot access memory at address 0x433b6e69625c7469>}}
    //TODO: id works fine
    Student& operator= (const Student &sdt)
    {
        Student temp;
        temp.first=sdt.first;
        temp.last=sdt.last;
        temp.id=sdt.id;
        return temp;
        first=sdt.first;
        last=sdt.last;
        id=sdt.id;
        return *this;
    }
*/
    /*Student & Student::operator = (const Student &s)
    {
        if(this != &s){
            first=s.getFirst();
        }
        return *this;
    }*/
};