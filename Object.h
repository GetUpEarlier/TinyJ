//
// Created by chenjs on 2019/10/22.
//

#ifndef TINYJ_OBJECT_H
#define TINYJ_OBJECT_H

#include "Value.h"
#include "Slot.h"

namespace TinyJ{

class Class;

class ObjectHeader{
public:
    Class* klass;
    Int hashCode;

    U8* content(){
        return (U8*)(this+1);
    }
    bool isInstance(Class* targetKlass);

    /*template <typename T>
    Bytes<sizeof(T)>& field(size_t shift){
        return *(Bytes<sizeof(T)>*)(&((U8*)(this+1))[shift]);
    }*/
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

#endif //TINYJ_OBJECT_H
