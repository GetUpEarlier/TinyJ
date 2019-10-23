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

    Slot& slotAt(U32 index){
        return ((Slot*)(this+1))[index];
    }

    Slot2& slot2At(U32 index){
        return *((Slot2*)&((Slot*)(this+1))[index]);
    }
};

}

#endif //TINYJ_ARRAY_H
