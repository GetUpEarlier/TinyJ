//
// Created by chenjs on 2019/10/24.
//

#include "Value.h"

namespace TinyJ{

U32 getSizeFromDescriptor(string descriptor) {
    if(descriptor == "V"){
        return 0;
    }
    if(parsePrimitiveFromDescriptor(descriptor).has_value()){
        return primitiveSize(parsePrimitiveFromDescriptor(std::move(descriptor)).value());
    }else{
        return sizeof(Reference);
    }
}

U32 getSlotsFromDescriptor(string descriptor) {
    U32 size = getSizeFromDescriptor(std::move(descriptor));
    return (size + (sizeof(Slot)-1)) / sizeof(Slot);
}

U32 primitiveSize(Primitive primitive) {
    switch (primitive){
        case Primitive::BOOLEAN:
            return sizeof(Boolean);
        case Primitive::CHAR:
            return sizeof(Char);
        case Primitive::BYTE:
            return sizeof(Byte);
        case Primitive::SHORT:
            return sizeof(Short);
        case Primitive::INT:
            return sizeof(Int);
        case Primitive::FLOAT:
            return sizeof(Float);
        case Primitive::LONG:
            return sizeof(Long);
        case Primitive::DOUBLE:
            return sizeof(Double);
        default:
            throw std::exception();
    }
}

string primitiveDescriptor(Primitive primitive) {
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

optional<Primitive> parsePrimitiveFromDescriptor(string className) {
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

}
