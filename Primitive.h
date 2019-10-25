//
// Created by chenjs on 2019/10/21.
//

#ifndef TINYJ_PRIMITIVE_H
#define TINYJ_PRIMITIVE_H

#include <memory>

#include "Number.h"

namespace TinyJ{

using Boolean = UnsignedValue<1>;
using Byte = SignedValue<1>;
using Char = UnsignedValue<2>;
using Short = SignedValue<2>;
using Int = SignedValue<4>;
using Long = SignedValue<8>;
using Float = FloatValue<4>;
using Double = FloatValue<8>;
using Reference = UnsignedValue<4>;

using U8 = uint8_t;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;

using I8 = int8_t;
using I16 = int16_t;
using I32 = int32_t;
using I64 = int64_t;

using F32 = float;
using F64 = double;


enum class Encoding{
    BIG_ENCODING, LITTLE_ENCODING
};

template <size_t Length, Encoding encoding = Encoding::BIG_ENCODING>
struct Bytes{

};

//template <size_t Length>
template <size_t Length>
struct Bytes<Length, Encoding::BIG_ENCODING>{
    Byte bytes[Length];

    UnsignedValue<Length> getUnsigned(){
        UnsignedValue<Length> value = 0;
        for(size_t i = 0;i < Length;i++){
            value <<= 8u;
            value |= bytes[i];
        }
        return value;
    }

    SignedValue<Length> getSigned(){
        UnsignedValue<Length> value = getUnsigned();
        return *(SignedValue<Length>*)&value;
    }

    FloatValue<Length> getFloat(){
        UnsignedValue<Length> value = getUnsigned();
        return *(FloatValue<Length>*)&value;
    }

    void setUnsigned(UnsignedValue<Length> value){
        for(size_t i = Length-1;i >= 0;i--){
            bytes[i] = value & 0xFF;
            value = value >> 8u;
        }
    }

    void setSigned(SignedValue<Length> value){
        setUnsigned(*(UnsignedValue<Length>*)&value);
    }

    void setFloat(FloatValue<Length> value){
        setUnsigned(*(UnsignedValue<Length>*)&value);
    }

    void set(UnsignedValue<Length> value){
        setUnsigned(value);
    }

    void set(SignedValue<Length> value){
        setSigned(value);
    }

    void set(FloatValue<Length> value){
        setFloat(value);
    }
};



/*using StackChar = uint16_t;
using StackShort = int16_t;
using StackInt = int32_t;
using StackLong = int64_t;
using StackFloat = float;
using StackDouble = double;
using StackReference = uint32_t;
using StackU32 = uint32_t;

using Byte = uint8_t;
using Boolean = Byte;

struct Char{
    Byte b0;
    Byte b1;
};

struct Short{
    Byte b0;
    Byte b1;
};

struct Int{
    Byte b0;
    Byte b1;
    Byte b2;
    Byte b3;
};

struct Reference{
    Byte b0;
    Byte b1;
    Byte b2;
    Byte b3;
};

struct Long{
    Byte b0;
    Byte b1;
    Byte b2;
    Byte b3;
    Byte b4;
    Byte b5;
    Byte b6;
    Byte b7;
};

struct Float{
    Byte b0;
    Byte b1;
    Byte b2;
    Byte b3;
};

struct Double{
    Byte b0;
    Byte b1;
    Byte b2;
    Byte b3;
    Byte b4;
    Byte b5;
    Byte b6;
    Byte b7;
};

struct U32{
    Byte b0;
    Byte b1;
    Byte b2;
    Byte b3;
    Byte b4;
    Byte b5;
    Byte b6;
    Byte b7;
};*/

}

#endif //TINYJ_PRIMITIVE_H
