//
// Created by chenjs on 2019/10/23.
//

#include "Heap.h"

#include "Object.h"
#include "Class.h"

namespace TinyJ{

/*Reference Heap::allocObject(U32 size) {
    Reference reference = _allocated;
    U8* memory = _memory;
    _allocated += sizeof(Object) + size;
    auto* object = (Object*)memory;
    return _allocated;
}

Reference Heap::newObject(Class* klass) {
    Reference reference = allocObject(klass->objectSize());
    derefObject(reference)->klass = klass;
    return reference;
}

Reference Heap::newArray(Class* arrayKlass, U32 length) {
    Reference reference = allocObject(length*sizeof(Reference)+ sizeof(U32));
    derefObject(reference)->klass = arrayKlass;
    return reference;
}

Object* Heap::derefObject(Reference reference) {
    return ((Object*)&_memory[reference]);
}

Reference Heap::refObject(Object* object) {
    return ((U8*)object) - _memory;
}*/

}