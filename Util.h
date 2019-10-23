//
// Created by chenjs on 2019/10/21.
//

#ifndef TINYJ_UTIL_H
#define TINYJ_UTIL_H

namespace TinyJ{

template <typename T>
void* tailPtr(T* ptr){
    ptr++;
    return static_cast<void*>(ptr);
}

}

#endif //TINYJ_UTIL_H
