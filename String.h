//
// Created by chenjs on 2019/10/14.
//

#ifndef TINYJ_STRING_H
#define TINYJ_STRING_H

/*#include <memory>
#include "Char.h"

using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;

using std::make_shared;
using std::make_unique;
using std::make_pair;

class String {
private:
    shared_ptr<wchar_t[]> _chars;
    int _length;
public:
    String(const wchar_t* chars){
        int length = 0;
        const wchar_t* ptr = chars;
        while (*ptr++ != 0){
            length++;
        }
        _length = length;
        _chars = shared_ptr<wchar_t[]>(new wchar_t[length + 1]);
        for(int i = 0;i < length;i++){
            _chars[i] = chars[i];
        }
        _chars[length] = 0;
    }
    [[nodiscard]] Char charAt(int descriptorIndex) const{
        return Char(_chars[descriptorIndex]);
    }
    [[nodiscard]] int size() const{
        return _length;
    }
    [[nodiscard]] const char* c_str() const{
        char* chars = new char[_length+1];
        for(size_t i = 0;i < _length;i++){
            chars[i] = (char)_chars[i];
        }
        chars[_length] = '\0';
        return chars;
    }
};*/


#endif //TINYJ_STRING_H
