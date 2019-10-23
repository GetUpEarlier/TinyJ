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

    bool isInstance(Class* targetKlass);
    Slot& slotAt(U32 shift){
        return ((Slot*)(this+1))[shift];
    }
    Slot2& slot2At(U32 shift){
        return *((Slot2*)&((Slot*)(this+1))[shift]);
    }
};

}

#endif //TINYJ_OBJECT_H
