//
// Created by chenjs on 2019/10/22.
//

#ifndef TINYJ_SLOT_H
#define TINYJ_SLOT_H

#include <memory>
#include "Primitive.h"

namespace TinyJ{

union Slot{
    Boolean boolValue;
    Int intValue;
    Short shortValue;
    Byte byteValue;
    Char charValue;
    Float floatValue;
    Reference referenceValue;

    static Slot fromBoolean(Boolean value){
        Slot slot{};
        slot.boolValue = value;
        return slot;
    }
    static Slot fromInt(Int value){
        Slot slot{};
        slot.intValue = value;
        return slot;
    }
    static Slot fromFloat(Float value){
        Slot slot{};
        slot.floatValue = value;
        return slot;
    }
    static Slot fromShort(Short value){
        Slot slot{};
        slot.shortValue = value;
        return slot;
    }
    static Slot fromByte(Byte value){
        Slot slot{};
        slot.byteValue = value;
        return slot;
    }
    static Slot fromChar(Char value){
        Slot slot{};
        slot.charValue = value;
        return slot;
    }
    static Slot fromReference(Reference value){
        Slot slot{};
        slot.referenceValue = value;
        return slot;
    }
};

struct SlotX2{
    Slot low;
    Slot high;
};

union Slot2{
    /*struct {
        Slot low;
        Slot high;
    };*/
    Long longValue;
    Double doubleValue;

    static Slot2 fromLong(Long value){
        Slot2 slot{};
        slot.longValue = value;
        return slot;
    }

    static Slot2 fromDouble(Double value){
        Slot2 slot{};
        slot.doubleValue = value;
        return slot;
    }

    /*static Slot2 fromSlots(Slot low, Slot high){
        Slot2 slot{};
        slot.low = low;
        slot.high = high;
        return slot;
    }*/
};

}

#endif //TINYJ_SLOT_H
