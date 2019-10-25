//
// Created by chenjs on 2019/10/23.
//

#ifndef TINYJ_STATICTABLE_H
#define TINYJ_STATICTABLE_H

#include "Primitive.h"
#include "Slot.h"

namespace TinyJ{

class StaticTable{
private:
    U8* _content;
    U32 _capacity;
public:
    explicit StaticTable(U32 capacity){
        _content = new U8[capacity];
        _capacity = capacity;
    }

    U32 capacity(){
        return _capacity;
    }

    U8* content(){
        return _content;
    }
    Boolean& booleanAt(U32 shift){
        return *(Boolean*)(content() + shift);
    }
    Byte& byteAt(U32 shift){
        return *(Byte*)(content() + shift);
    }
    Short& shortAt(U32 shift){
        return *(Short*)(content() + shift);
    }
    Char& charAt(U32 shift){
        return *(Char*)(content() + shift);
    }
    Int& intAt(U32 shift){
        return *(Int*)(content() + shift);
    }
    Long& longAt(U32 shift){
        return *(Long*)(content() + shift);
    }
    Float& floatAt(U32 shift){
        return *(Float*)(content() + shift);
    }
    Double& doubleAt(U32 shift){
        return *(Double*)(content() + shift);
    }
    Reference& referenceAt(U32 shift){
        return *(Reference*)(content() + shift);
    }
};

}

#endif //TINYJ_STATICTABLE_H
