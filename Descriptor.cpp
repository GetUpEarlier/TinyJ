//
// Created by chenjs on 2019/10/23.
//

#include "Descriptor.h"

namespace TinyJ{

string readDescriptor(string input, size_t& cursor){
    char first = input[cursor++];
    switch(first){
        case 'V':
            return "V";
        case 'Z':
            return "Z";
        case 'C':
            return "C";
        case 'F':
            return "F";
        case 'D':
            return "D";
        case 'B':
            return "B";
        case 'S':
            return "S";
        case 'I':
            return "I";
        case 'J':
            return "J";
        case 'L':{
            string buffer;
            buffer.push_back('L');
            while(true){
                char ch = input[cursor++];
                if(ch == ';'){
                    break;
                }else{
                    buffer.push_back(ch);
                }
            }
            return buffer;
        }
        default:{
            throw std::exception();
        }
    }
}

}