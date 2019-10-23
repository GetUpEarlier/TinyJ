//
// Created by chenjs on 2019/10/21.
//

#ifndef TINYJ_ENDIAN_H
#define TINYJ_ENDIAN_H

#include <array>
#include "Primitive.h"

namespace TinyJ{

using std::array;

class Endian{
public:
    virtual U16 toU16(U8 array[2]) = 0;
    virtual U32 toU32(U8 array[4]) = 0;
    virtual U64 toU64(U8 array[8]) = 0;
    virtual void toBytes(U64 value, U8 buffer[8]) = 0;
    virtual void toBytes(U32 value, U8 buffer[4]) = 0;
    virtual void toBytes(U16 value, U8 buffer[2]) = 0;
};

class BigEndian: public Endian{
public:
    U16 toU16(U8* array) override {
        U16 result = 0;
        for(size_t i = 0;i < 2;i++){
            result <<= 8u;
            result |= array[i];
        }
        return result;
    }

    U32 toU32(U8* array) override {
        U32 result = 0;
        for(size_t i = 0;i < 4;i++){
            result <<= 8u;
            result |= array[i];
        }
        return result;
    }

    U64 toU64(U8* array) override {
        U64 result = 0;
        for(size_t i = 0;i < 8;i++){
            result <<= 8u;
            result |= array[i];
        }
        return result;
    }

    void toBytes(U64 value, U8* buffer) override {
        for(size_t i = 0;i < 8;i++){
            buffer[8-i-1] = value & 0xFFu;
            value >>= 8u;
        }
    }

    void toBytes(U32 value, U8* buffer) override {
        for(size_t i = 0;i < 4;i++){
            buffer[4-i-1] = value & 0xFFu;
            value >>= 8u;
        }
    }

    void toBytes(U16 value, U8* buffer) override {
        for(size_t i = 0;i < 2;i++){
            buffer[2-i-1] = value & 0xFFu;
            value >>= 8u;
        }
    }
};

class LittleEndian: public Endian{
public:
    U16 toU16(U8* array) override {
        U16 result = 0;
        for(size_t i = 0;i < 2;i++){
            result <<= 8u;
            result |= array[2-i-1];
        }
        return result;
    }

    U32 toU32(U8* array) override {
        U32 result = 0;
        for(size_t i = 0;i < 4;i++){
            result <<= 8u;
            result |= array[4-i-1];
        }
        return result;
    }

    U64 toU64(U8* array) override {
        U64 result = 0;
        for(size_t i = 0;i < 8;i++){
            result <<= 8u;
            result |= array[8-i-1];
        }
        return result;
    }

    void toBytes(U64 value, U8* buffer) override {
        for(size_t i = 0;i < 8;i++){
            buffer[i] = value & 0xFFu;
            value >>= 8u;
        }
    }

    void toBytes(U32 value, U8* buffer) override {
        for(size_t i = 0;i < 4;i++){
            buffer[i] = value & 0xFFu;
            value >>= 8u;
        }
    }

    void toBytes(U16 value, U8* buffer) override {
        for(size_t i = 0;i < 2;i++){
            buffer[i] = value & 0xFFu;
            value >>= 8u;
        }
    }
};

}

#endif //TINYJ_ENDIAN_H
