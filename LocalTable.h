//
// Created by chenjs on 2019/10/22.
//

#ifndef TINYJ_LOCALTABLE_H
#define TINYJ_LOCALTABLE_H

#include "Primitive.h"

namespace TinyJ{

class LocalTable{
private:
    Slot* _slots;
    U32 _capacity;
public:
    explicit LocalTable(U32 capacity){
        _slots = new Slot[capacity];
        _capacity = capacity;
    }

    U32 capacity(){
        return _capacity;
    }

    Slot& at(U32 index){
        return _slots[index];
    }

    Slot2& at2(U32 index){
        return *((Slot2*)(&_slots[index]);
    }
};

}

#endif //TINYJ_LOCALTABLE_H
