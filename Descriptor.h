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
    U32 retSlotCount{};

    bool operator==(const MethodDescriptor& another){
        return retDescriptor == another.retDescriptor && argDescriptors == another.argDescriptors;
    }
};

string readDescriptor(string input, size_t& cursor);

MethodDescriptor* parseMethodDescriptor(string input);

}

#endif //TINYJ_DESCRIPTOR_H
