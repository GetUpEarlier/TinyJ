//
// Created by chenjs on 2019/10/22.
//

#ifndef TINYJ_BYTEBUFFER_H
#define TINYJ_BYTEBUFFER_H

#include "Primitive.h"

namespace TinyJ{

template <typename TEndian>
class ByteBuffer{
private:
    U8* _buffer;
    size_t _size;
    size_t _frontPtr;
    size_t _backPtr;
    TEndian endian;
public:
    explicit ByteBuffer(U8* buffer, size_t size, size_t frontPtr, size_t backPtr){
        _buffer = buffer;
        _size = size;
        _frontPtr = frontPtr;
        _backPtr = backPtr;
    }
    U8 readU8(){
        return _buffer[_frontPtr++];
    }
    U16 readU16(){
        U16 value = endian.toU16(_buffer + _frontPtr);
        _frontPtr += 2;
        return value;
    }
    U32 readU32(){
        U32 value = endian.toU32(_buffer + _frontPtr);
        _frontPtr += 4;
        return value;
    }
    U64 readU64(){
        U64 value = endian.toU64(_buffer + _frontPtr);
        _frontPtr += 8;
        return value;
    }
    void writeU8(U8 value){
        _buffer[_frontPtr++] = value;
    }
    void writeU16(U16 value){
        endian.toBytes(value, _buffer + _backPtr);
        _backPtr += 2;
    }
    void writeU32(U32 value){
        endian.toBytes(value, _buffer + _backPtr);
        _backPtr += 4;
    }
    void writeU64(U64 value){
        endian.toBytes(value, _buffer + _backPtr);
        _backPtr += 8;
    }
};

}

#endif //TINYJ_BYTEBUFFER_H
