//
// Created by chenjs on 2019/10/21.
//

#ifndef TINYJ_INPUTSTREAM_H
#define TINYJ_INPUTSTREAM_H

#include <iostream>
#include "Endian.h"

namespace TinyJ{

using std::istream;

class InputStream{
private:
    Endian* _endian;
    istream* _stream;
    U8* _buffer;
public:
    explicit InputStream(istream* stream, Endian* endian){
        _stream = stream;
        _endian = endian;
        _buffer = new U8[8];
        for(size_t i = 0;i < 8;i++){
            _buffer[i] = 0;
        }
    }
    U8 readU8(){
        _stream->read((char*)_buffer, 1);
        return _buffer[0];
    }
    U16 readU16(){
        _stream->read((char*)_buffer, 2);
        return _endian->toU16(_buffer);
    }
    U32 readU32(){
        _stream->read((char*)_buffer, 4);
        return _endian->toU32(_buffer);
    }
    U64 readU64(){
        _stream->read((char*)_buffer, 8);
        return _endian->toU64(_buffer);
    }
    U8* readN(size_t n){
        U8* buffer = new U8[n];
        _stream->read((char*)buffer, n);
        return buffer;
    }
};

}

#endif //TINYJ_INPUTSTREAM_H
