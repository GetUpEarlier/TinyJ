//
// Created by chenjs on 2019/10/22.
//

#ifndef TINYJ_CODEREADER_H
#define TINYJ_CODEREADER_H

#include "Primitive.h"
#include "Instruction.h"

namespace TinyJ{

class CodeReader{
private:
    U8* _array;
    U32 _length;
    U32* _pc;
public:
    explicit CodeReader(U8* array, U32 length, U32* pc){
        _array = array;
        _length = length;
        _pc = pc;
    }

    U32 pc(){
        return *_pc;
    }

    void setPc(U32 pc){
        *_pc = pc;
    }

    void incPc(U32 inc){
        *_pc += inc;
    }

    U8 readU8(){
        return _array[(*_pc)++];
    }

    OpCode readOpCode(){
        return (OpCode)readU8();
    }

    Byte readByte(){
        return (Byte)readU8();
    }

    Short readShort(){
        return (Short)readU16();
    }

    Int readInt(){
        U8 b1 = readU8();
        U8 b2 = readU8();
        U8 b3 = readU8();
        U8 b4 = readU8();
        U32 value = b1;
        value <<= 8u;
        value += b2;
        value <<= 8u;
        value += b3;
        value <<= 8u;
        value += b4;
        return (Int)value;
    }

    U16 readU16(){
        U8 low = readU8();
        U8 high = readU8();
        U16 value = 0;
        value |= low;
        value <<= 8u;
        value |= high;
        return value;
    }


};

}

#endif //TINYJ_CODEREADER_H
