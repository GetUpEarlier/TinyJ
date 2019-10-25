//
// Created by chenjs on 2019/10/23.
//

#ifndef TINYJ_ARRAY_H
#define TINYJ_ARRAY_H

#include <cassert>
#include "Class.h"
#include "Slot.h"

namespace TinyJ{

class ArrayHeader{
public:
    Class* klass;
    U32 length;

    Class* elementClass(){
        assert(klass->isArray);
        return klass->getElementClass();
    }

    template <typename T>
    T* element(){
        return (T*)(this+1);
    }
    Boolean& booleanElement(U32 index){
        return element<Boolean>()[index];
    }
    Byte& byteElement(U32 index){
        return element<Byte>()[index];
    }
    Char& charElement(U32 index){
        return element<Char>()[index];
    }
    Short& shortElement(U32 index){
        return element<Short>()[index];
    }
    Int& intElement(U32 index){
        return element<Int>()[index];
    }
    Long& longElement(U32 index){
        return element<Long>()[index];
    }
    Double& doubleElement(U32 index){
        return element<Double>()[index];
    }
    Float& floatElement(U32 index){
        return element<Float>()[index];
    }
    Reference& referenceElement(U32 index){
        return element<Reference>()[index];
    }

};

}

#endif //TINYJ_ARRAY_H
