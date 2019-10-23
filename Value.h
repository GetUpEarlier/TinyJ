//
// Created by chenjs on 2019/10/22.
//

#ifndef TINYJ_VALUE_H
#define TINYJ_VALUE_H

#include <memory>
#include <optional>
#include <string>

#include "Primitive.h"

namespace TinyJ{

using std::optional;
using std::string;

class ObjectHeader;

enum class Primitive{
    NONE = 0,
    BOOLEAN = 4,
    CHAR = 5,
    FLOAT = 6,
    DOUBLE = 7,
    BYTE = 8,
    SHORT = 9,
    INT = 10,
    LONG = 11,
};

optional<Primitive> parsePrimitiveFromDescriptor(string className){
    if(className == "Z"){
        return Primitive::BOOLEAN;
    }else if(className == "C"){
        return Primitive::CHAR;
    }else if(className == "F"){
        return Primitive::FLOAT;
    }else if(className == "D"){
        return Primitive::DOUBLE;
    }else if(className == "B"){
        return Primitive::BYTE;
    }else if(className == "S"){
        return Primitive::SHORT;
    }else if(className == "I"){
        return Primitive::INT;
    }else if(className == "J"){
        return Primitive::LONG;
    }else return optional<Primitive>();
}

string primitiveDescriptor(Primitive primitive){
    switch (primitive){
        case Primitive::BOOLEAN:
            return "Z";
        case Primitive::CHAR:
            return "C";
        case Primitive::FLOAT:
            return "F";
        case Primitive::DOUBLE:
            return "D";
        case Primitive::BYTE:
            return "B";
        case Primitive::SHORT:
            return "S";
        case Primitive::INT:
            return "I";
        case Primitive::LONG:
            return "J";
        default:
            return "";
    }
}

U32 primitiveSize(Primitive primitive){
    switch (primitive){
        case Primitive::BOOLEAN:
        case Primitive::CHAR:
        case Primitive::BYTE:
        case Primitive::SHORT:
        case Primitive::INT:
        case Primitive::FLOAT:
            return 32;
        case Primitive::LONG:
        case Primitive::DOUBLE:
            return 64;
        default:
            throw std::exception();
    }
}

U32 getSizeFromDescriptor(string descriptor){
    if(parsePrimitiveFromDescriptor(descriptor).has_value()){
        return primitiveSize(parsePrimitiveFromDescriptor(std::move(descriptor)).value());
    }else{
        return 32;
    }
}

/*union Value{
    bool booleanValue;
    int8_t byteValue;
    int16_t shortValue;
    int32_t intValue;
    int64_t longValue;
    float floatValue;
    double doubleValue;
    Object* objectValue;
    U8 u8Value;
    U16 u16Value;
    U32 u32Value;
    U64 u64Value;

    static Value fromU8(U8 u8Value){
        Value value{};
        value.u8Value = u8Value;
        return value;
    }
    static Value fromU16(U16 u16Value){
        Value value{};
        value.u16Value = u16Value;
        return value;
    }
    static Value fromU32(U32 u32Value){
        Value value{};
        value.u32Value = u32Value;
        return value;
    }
    static Value fromU64(U64 u64Value){
        Value value{};
        value.u64Value = u64Value;
        return value;
    }
    static Value fromI32(int32_t i32Value){
        Value value{};
        value.intValue = i32Value;
        return value;
    }
    static Value fromI64(int64_t i64Value){
        Value value{};
        value.longValue = i64Value;
        return value;
    }
    static Value fromF32(float floatValue){
        Value value{};
        value.floatValue = floatValue;
        return value;
    }
    static Value fromF64(double doubleValue){
        Value value{};
        value.doubleValue = doubleValue;
        return value;
    }
    static Value fromObject(Object* object){
        Value value{};
        value.objectValue = object;
        return value;
    }
};*/

}

#endif //TINYJ_VALUE_H
