//
// Created by chenjs on 2019/10/22.
//

#ifndef TINYJ_VALUE_H
#define TINYJ_VALUE_H

#include <memory>
#include <optional>
#include <string>

#include "Primitive.h"
#include "Slot.h"

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

optional<Primitive> parsePrimitiveFromDescriptor(string className);

string primitiveDescriptor(Primitive primitive);

U32 primitiveSize(Primitive primitive);

U32 getSizeFromDescriptor(string descriptor);

U32 getSlotsFromDescriptor(string descriptor);

}

#endif //TINYJ_VALUE_H
