//
// Created by chenjs on 2019/10/22.
//

#ifndef TINYJ_OPERANDSTACK_H
#define TINYJ_OPERANDSTACK_H

#include "Slot.h"

namespace TinyJ{

class OperandStack{
private:
    Slot* _stack;
    U32 _capacity;
    Slot* _top;

public:
    explicit OperandStack(U32 capacity){
        _stack = new Slot[capacity];
        _capacity = capacity;
        _top = _stack;
    }

    Slot& top(){
        return *_top;
    }
    Slot pop(){
        return *(_top--);
    }
    void push(Slot slot){
        *(++_top) = slot;
    }
    U32 size(){
        return _top - _stack;
    }
    U32 capacity(){
        return _capacity;
    }
    Slot2& top2(){
        return *((Slot2*)(_top-1));
    }
    Slot2 pop2(){
        Slot2 value = top2();
        _top -= 2;
        return value;
    }
    void pushByte(Byte value){
        push(Slot::fromByte(value));
    }
    void pushShort(Short value){
        push(Slot::fromShort(value));
    }
    void pushChar(Char value){
        push(Slot::fromChar(value));
    }
    void pushInt(Int value){
        push(Slot::fromInt(value));
    }
    void pushFloat(Float value){
        push(Slot::fromFloat(value));
    }
    void pushReference(Reference value){
        push(Slot::fromReference(value));
    }
    void push2(Slot2 value){
        *(Slot2*)(_top-1) = value;
        _top += 2;
    }
    /*void push2(Slot low, Slot high){
        *(Slot2*)(_top-1) = Slot2::fromSlots(low, high);
        _top += 2;
    }*/
    void pushLong(Long value){
        push2(Slot2::fromLong(value));
    }
    void pushDouble(Double value){
        push2(Slot2::fromDouble(value));
    }
};

}

#endif //TINYJ_OPERANDSTACK_H
