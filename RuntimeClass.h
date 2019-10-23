//
// Created by chenjs on 2019/10/22.
//

#ifndef TINYJ_RUNTIMECLASS_H
#define TINYJ_RUNTIMECLASS_H

#include <vector>
#include <string>
#include "Primitive.h"
#include "Class.h"

namespace TinyJ{

/*using std::vector;
using std::string;


class RuntimeConstantInfo{
    ConstantType tag;
};

class RuntimeFieldInfo{

};

class RuntimeMethodInfo{

};

class RuntimeAttributeInfo{

};

struct RuntimeClass {
    vector<RuntimeConstantInfo*> constantPool;
    AccessFlag accessFlags;
    RuntimeClass* thisClass;
    RuntimeClass* superClass;
    vector<RuntimeClass*> interfaces;
    vector<RuntimeFieldInfo*> fields;
    vector<RuntimeMethodInfo*> methods;
    vector<RuntimeAttributeInfo*> attributes;

    //RUNTIME VALUES
    string name;
    Slot* staticFields;
    ClassLoader* classLoader;
    Class* elementClass;
    U32 refSize;
    U32 objectSize;
    bool isArray;
};

struct RuntimeUtf8Constant: RuntimeConstantInfo{
    string bytes;

    bool stringEquals(const char* string){
        for(size_t i = 0;i < bytes.size();i++){
            if(string[i] == '\0'){
                return false;
            }
            if(bytes[i] != string[i]){
                return false;
            }
        }
        return string[bytes.size()] == '\0';
    }
};

struct RuntimeIntConstant: RuntimeConstantInfo{
    Int value;
};

struct RuntimeFloatConstant: RuntimeConstantInfo{
    Float value;
};

struct RuntimeLongConstant: RuntimeConstantInfo{
    Long value;
};

struct RuntimeDoubleConstant: RuntimeConstantInfo{
    Double value;
};

struct RuntimeClassConstant: RuntimeConstantInfo{
    RuntimeClass* klass;
};

struct RuntimeStringConstant: RuntimeConstantInfo{

};*/

}


#endif //TINYJ_RUNTIMECLASS_H
