//
// Created by chenjs on 2019/10/22.
//

#ifndef TINYJ_HEAP_H
#define TINYJ_HEAP_H

#include "Primitive.h"

namespace TinyJ{

class ObjectHeader;
class Class;

class Heap{
private:
    U8* _memory;
    U32 _length;
    U32 _allocated;
public:
    explicit Heap(U8* memory, U32 length){
        _memory = memory;
        _length = length;
        _allocated = 8;
    }

    explicit Heap(U32 length){
        _memory = new U8[length];
        _length = length;
        _allocated = 8;
    }

//    Reference allocObject(U32 size);
//    Reference newObject(Class* klass);
//    Reference newArray(Class* klass, U32 length);
//    Object* derefObject(Reference reference);
//    Reference refObject(Object* object);
    Reference alloc(U32 size){
        Reference reference = _allocated;
        _allocated += size;
        return reference;
    }
    template <typename T>
    Reference alloc(){
        return alloc(sizeof(T));
    }
    void* deref(Reference reference){
        if(reference == 0){
            return nullptr;
        }
        return (void*)&_memory[reference];
    }
    template <typename T>
    T* deref(Reference reference){
        if(reference == 0){
            return nullptr;
        }
        return (T*)&_memory[reference];
    }
    Reference ref(void* ptr){
        if(ptr == nullptr){
            return 0;
        }
        return (U8*)ptr - _memory;
    }
};

}

#endif //TINYJ_HEAP_H
