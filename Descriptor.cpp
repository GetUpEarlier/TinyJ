//
// Created by chenjs on 2019/10/23.
//

#include "Descriptor.h"

namespace TinyJ{

string readDescriptor(string input, size_t& cursor){
    string array;
    while(true){
        char ch = input[cursor++];
        switch(ch){
            case 'V':
                return array + "V";
            case 'Z':
                return array + "Z";
            case 'C':
                return array + "C";
            case 'F':
                return array + "F";
            case 'D':
                return array + "D";
            case 'B':
                return array + "B";
            case 'S':
                return array + "S";
            case 'I':
                return array + "I";
            case 'J':
                return array + "J";
            case 'L':{
                string buffer;
                buffer.push_back('L');
                while(true){
                    ch = input[cursor++];
                    if(ch == ';'){
                        break;
                    }else{
                        buffer.push_back(ch);
                    }
                }
                return array + buffer;
            }
            case '[':{
                array.push_back('[');
                break;
            }
            default:{
                throw std::exception();
            }
        }
    }
}

MethodDescriptor* parseMethodDescriptor(string input) {
    auto* methodDescriptor = new MethodDescriptor();
    size_t cursor = 1;
    U32 slotSum = 0;
    while(true){
        if(input[cursor] == ')'){
            break;
        }else{
            string descriptor = readDescriptor(input, cursor);
            U32 argSlot = getSlotsFromDescriptor(descriptor);
            slotSum += argSlot;
            methodDescriptor->argDescriptors.push_back(descriptor);
        }
    }
    cursor++;
    methodDescriptor->retDescriptor = readDescriptor(input, cursor);
    methodDescriptor->argSlotCount = slotSum;
    methodDescriptor->retSlotCount = getSlotsFromDescriptor(methodDescriptor->retDescriptor);
    return methodDescriptor;
}

}