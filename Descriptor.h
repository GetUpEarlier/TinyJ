//
// Created by chenjs on 2019/10/23.
//

#ifndef TINYJ_DESCRIPTOR_H
#define TINYJ_DESCRIPTOR_H

#include <vector>
#include <string>

#include "Primitive.h"
#include "Value.h"

namespace TinyJ{

using std::vector;
using std::string;

struct MethodDescriptor{
    string retDescriptor;
    vector<string> argDescriptors;
    U32 argSlotCount{};

    bool operator==(const MethodDescriptor& another){
        return retDescriptor == another.retDescriptor && argDescriptors == another.argDescriptors;
    }
};

string readDescriptor(string input, size_t& cursor);

MethodDescriptor* parseMethodDescriptor(string input){
    auto* methodDescriptor = new MethodDescriptor();
    size_t cursor = 1;
    U32 sizeSum = 0;
    while(true){
        if(input[cursor] == ')'){
            break;
        }else{
            string descriptor = readDescriptor(input, cursor);
            U32 argSize = getSizeFromDescriptor(descriptor);
            sizeSum += argSize;
            methodDescriptor->argDescriptors.push_back(descriptor);
        }
    }
    cursor++;
    methodDescriptor->retDescriptor = readDescriptor(input, cursor);
    sizeSum += getSizeFromDescriptor(methodDescriptor->retDescriptor);
    methodDescriptor->argSlotCount = sizeSum / 32;
    return methodDescriptor;
}

}

#endif //TINYJ_DESCRIPTOR_H
