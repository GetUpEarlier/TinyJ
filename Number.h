//
// Created by chenjs on 2019/10/24.
//

#ifndef TINYJ_NUMBER_H
#define TINYJ_NUMBER_H

#include <memory>

namespace TinyJ {

template<size_t Size>
class UnsignedDef{
public:
    using Value = unsigned;
};

template<>
class UnsignedDef<1> {
public:
    using Value = uint8_t;
};

template<>
class UnsignedDef<2> {
public:
    using Value = uint16_t;
};

template<>
class UnsignedDef<4> {
public:
    using Value = uint32_t;
};

template<>
class UnsignedDef<8> {
public:
    using Value = uint64_t;
};

template<size_t Size>
class SignedDef{
public:
    using Value = signed;
};

template<>
class SignedDef<1> {
public:
    using Value = int8_t;
};

template<>
class SignedDef<2> {
public:
    using Value = int16_t;
};

template<>
class SignedDef<4> {
public:
    using Value = int32_t;
};

template<>
class SignedDef<8> {
public:
    using Value = int64_t;
};

template <size_t Size>
class FloatDef{
public:
    using Value = float ;
};

template <>
class FloatDef<4>{
public:
    using Value = float;
};

template <>
class FloatDef<8>{
public:
    using Value = double;
};

template <size_t Size>
using UnsignedValue = typename UnsignedDef<Size>::Value;

template <size_t Size>
using SignedValue  = typename SignedDef<Size>::Value;

template <size_t Size>
using FloatValue = typename FloatDef<Size>::Value;

template <typename T>
class IsSigned;

template <>
class IsSigned<SignedValue<1>>{
    constexpr static bool isSigned = true;
};

template <>
class IsSigned<SignedValue<2>>{
    constexpr static bool isSigned = true;
};

template <>
class IsSigned<SignedValue<4>>{
    constexpr static bool isSigned = true;
};

template <>
class IsSigned<SignedValue<8>>{
    constexpr static bool isSigned = true;
};

template <>
class IsSigned<FloatValue<4>>{
    constexpr static bool isSigned = true;
};

template <>
class IsSigned<FloatValue<8>>{
    constexpr static bool isSigned = true;
};

template <typename T>
bool isSigned(){
    return IsSigned<T>::isSigned;
}

}


#endif //TINYJ_NUMBER_H
