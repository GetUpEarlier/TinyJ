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
    Slot* _slots;
    U32 _capacity;
public:
    explicit StaticTable(U32 capacity){
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
        return *((Slot2*)(&_slots[index]));
    }
};

}

#endif //TINYJ_STATICTABLE_H
