//
// Created by chenjs on 2019/10/16.
//

#ifndef TINYJ_CLASS_H
#define TINYJ_CLASS_H

#include <vector>
#include <string>
#include <functional>
#include "Primitive.h"
#include "String.h"
#include "Slot.h"
#include "Util.h"
#include "StaticTable.h"
#include "Value.h"
#include "Object.h"
#include "Descriptor.h"

namespace TinyJ{

using std::vector;
using std::string;

union Value;
class Class;
class ClassLoader;
class Interpreter;
class StackFrame;

using NativeMethod = std::function<void(vector<Slot>*)>;

enum class ConstantType: U8{
    UTF8 = 1,
    I32 = 3,
    F32 = 4,
    I64 = 5,
    F64 = 6,
    CLASS = 7,
    STRING = 8,
    FIELD_REF = 9,
    METHOD_REF = 10,
    INTERFACE_METHOD_REF = 11,
    NAME_AND_TYPE = 12,
    METHOD_HANDLE = 15,
    METHOD_TYPE = 16,
    DYNAMIC = 17,
    INVOKE_DYNAMIC = 18,
    MODULE = 19,
    PACKAGE = 20,

};

struct ClassMember{
    Class* klass;
};

struct ConstantInfo: ClassMember{
    ConstantType tag;
};

enum class AccessFlag: U16{
    PUBLIC = 0x0001,
    FINAL = 0x0010,
    SUPER = 0x0020,
    INTERFACE = 0x0200,
    ABSTRACT = 0x0400,
    SYNTHETIC = 0x1000,
    ANNOTATION = 0x2000,
    ENUM = 0x4000,
};

struct AttributeInfo: ClassMember{
    U16 nameIndex;

    //Runtime Value
    string name;
};

struct ExceptionInfo{
    U16 startPc;
    U16 endPc;
    U16 handlerPc;
    U16 catchType;
};

struct AttributeCode: public AttributeInfo{
    U16 maxStack;
    U16 maxLocals;
    U32 codeLength;
    U8* code;
    vector<ExceptionInfo*> exceptionTable;
    vector<AttributeInfo*> attributes;
};

struct AttributeExceptions: public AttributeInfo{
    vector<U16> exceptionIndexTable;
};

struct LineNumberInfo{
    U16 startPc;
    U16 lineNumber;
};

struct AttributeLineNumberTable: public AttributeInfo{
    vector<LineNumberInfo*> lineNumberTable;
};

struct LocalVariableInfo{
    U16 startPc;
    U16 length;
    U16 nameIndex;
    U16 descriptorIndex;
    U16 index;

    //Runtime Value
    string name;
    string descriptor;
    Class* klass;
};

struct AttributeLocalVariableTable: public AttributeInfo{
    vector<LocalVariableInfo*> localVariableTable;
};

struct AttributeSourceFile: public AttributeInfo{
    U16 sourceFileIndex;
};

struct AttributeConstantValue: public AttributeInfo{
    U16 constantValueIndex;
};

enum class InnerClassAccessFlag{
    PUBLIC = 0x000,
    PRIVATE = 0x0002,
    PROTECTED = 0x0004,
    STATIC = 0x0008,
    FINAL = 0x0010,
    INTERFACE = 0x0020,
    ABSTRACT = 0x0400,
    SYNTHETIC = 0x1000,
    ANNOTATION = 0x2000,
    ENUM = 0x4000
};

struct InnerClassesInfo{
    U16 innerClassInfoIndex;
    U16 outerClassInfoIndex;
    U16 innerNameIndex;
    InnerClassAccessFlag innerClassAccessFlag;

    //Runtime Value

};

struct AttributeInnerClasses: public AttributeInfo{
    vector<InnerClassesInfo*> innerClasses;
};

struct AttributeDeprecated: public AttributeInfo{

};

struct AttributeSynthetic: public AttributeInfo{

};

/*struct StackMapFrame{

};

struct AttributeStackMapTable: public AttributeInfo{
    vector<StackMapFrame*> stackMapFrameEntries;
};*/

struct AttributeSignature: public AttributeInfo{
    U16 signatureIndex;
};

struct UnknownAttribute: public AttributeInfo{
    U16 attributeLength;
    U8* attributeContent;
};

struct BootstrapMethod{
    U16 bootstrapMethodRef;
    vector<U16> bootstrapArguments;
};

struct AttributeBootstrapMethods: public AttributeInfo{
    vector<BootstrapMethod*> bootstrapMethods;
};

enum class FieldAccessFlag: U16{
    PUBLIC = 0x0001,
    PRIVATE = 0x0002,
    PROTECTED = 0x0004,
    STATIC = 0x0008,
    FINAL = 0x0010,
    VOLATILE = 0x0040,
    TRANSIENT = 0x0080,
    SYNTHETIC = 0x1000,
    ENUM = 0x4000
};

struct FieldInfo:ClassMember{
    FieldAccessFlag accessFlag;
    U16 nameIndex;
    U16 descriptorIndex;
    vector<AttributeInfo*> attributes;

    //Runtime Value
    string name;
    string descriptor;
    Class* klass;
    U32 shift;
    U32 size;

    Class* getClass();

    AttributeInfo* findAttribute(Class* context, const char* name);

    bool isStatic(){
        return ((U16)accessFlag & (U16)FieldAccessFlag::STATIC);
    }

};

enum class MethodAccessFlag: U16{
    PUBLIC = 0x0001,
    PRIVATE = 0x0002,
    PROTECTED = 0x0004,
    STATIC = 0x0008,
    FINAL = 0x0010,
    SYNCHRONIZED = 0x0020,
    BRIDGE = 0x0040,
    VARARGS = 0x0080,
    NATIVE = 0x0100,
    ABSTRACT = 0x0400,
    STRICTFP = 0x0800,
    SYNTHETIC = 0x1000,
};

struct MethodInfo:ClassMember{
    MethodAccessFlag accessFlag;
    U16 nameIndex;
    U16 descriptorIndex;
    vector<AttributeInfo*> attributes;

    //Runtime Value
    string name;
    string descriptor;
    MethodDescriptor* methodDescriptor;
    AttributeCode* code;
    NativeMethod* nativeMethod;

    bool isStatic(){
        return ((U16)accessFlag&(U16)MethodAccessFlag::STATIC);
    }

    bool isNative(){
        return ((U16)accessFlag&(U16)MethodAccessFlag::NATIVE);
    }

    AttributeInfo* findAttribute(Class* context, const char* name);
};



struct ConstantUtf8: ConstantInfo{
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

struct ConstantInteger: ConstantInfo{
    Int value;
};

struct ConstantFloat: ConstantInfo{
    Float value;
};

struct ConstantLong: ConstantInfo{
    Long value;
};

struct ConstantDouble: ConstantInfo{
    Double value;
};

struct ConstantClass: ConstantInfo{
    U16 nameIndex;

    //Runtime Value
    Class* klass;
    ConstantUtf8* name;

    Class* getClass();
    ConstantUtf8* getName();
};

struct ConstantString: ConstantInfo{
    U16 utf8Index;

    //Runtime Value
    ConstantUtf8* utf8;
    Reference reference;

    ConstantUtf8* getUtf8();
    Reference getReference(Interpreter* interpreter);
};

struct ConstantNameAndTypeRef: ConstantInfo{
    U16 nameIndex;
    U16 descriptorIndex;

    //Runtime Value
    ConstantUtf8* name;
    ConstantUtf8* descriptor;

    ConstantUtf8* getName();
    ConstantUtf8* getDescriptor();
};

struct ConstantFieldRef: ConstantInfo{
    U16 classIndex;
    U16 nameAndTypeIndex;

    //Runtime Value
    ConstantClass* klass;
    ConstantNameAndTypeRef* nameAndType;
    FieldInfo* fieldInfo;

    ConstantClass* getClass();
    ConstantNameAndTypeRef* getNameAndType();
    FieldInfo* getFieldInfo();
};

struct ConstantMethodRef: ConstantInfo{
    U16 classIndex;
    U16 nameAndTypeIndex;

    //Runtime Value
    ConstantClass* klass;
    ConstantNameAndTypeRef* nameAndType;
    MethodInfo* methodInfo;

    ConstantClass* getClass();
    ConstantNameAndTypeRef* getNameAndType();
    MethodInfo* getMethodInfo();
};

struct ConstantInterfaceMethodRef: ConstantInfo{
    U16 classIndex;
    U16 nameAndTypeIndex;

    //Runtime Value
    ConstantClass* klass;
    ConstantNameAndTypeRef* nameAndType;

    ConstantClass* getClass();
    ConstantNameAndTypeRef* getNameAndType();
};

struct ConstantMethodHandle: ConstantInfo{
    U8 referenceKind;
    U8 referenceIndex;
};

struct ConstantMethodType: ConstantInfo{
    U16 descriptorIndex;
};

struct ConstantInvokeDynamic: ConstantInfo{
    U16 bootStrapMethodAttr;
    U16 nameAndTypeIndex;
};

enum class ClassStatus{
    INITED, UN_INITED
};

struct Class{
    U32 magic;
    U16 minorVersion;
    U16 majorVersion;
    vector<ConstantInfo*> constantPool;
    AccessFlag accessFlags;
    U16 thisClassNameIndex;
    U16 superClassNameIndex;
    vector<U16> interfaceIndices;
    vector<FieldInfo*> fields;
    vector<MethodInfo*> methods;
    vector<AttributeInfo*> attributes;

    //RUNTIME VALUES
    string descriptor;
    StaticTable* staticTable;
    ClassLoader* classLoader;
    Class* elementClass;
    Class* thisClass;
    Class* superClass;
    vector<Class*> interfaces;
    U32 size;
    U32 objectSize;
    bool isArray;
    bool isPrimitive;
    Primitive primitive;
    ClassStatus status;

    Class* getElementClass();

    ConstantInfo* constant(size_t index){
        return constantPool[index - 1];
    }

    ConstantUtf8* constantString(size_t index){
        return (ConstantUtf8*)constant(index);
    }

    ConstantFieldRef* constantField(size_t index){
        return (ConstantFieldRef*)constant(index);
    }

    ConstantMethodRef* constantMethod(size_t index){
        return (ConstantMethodRef*)constant(index);
    }

    ConstantClass* constantClass(size_t index){
        return (ConstantClass*)constant(index);
    }

    ConstantNameAndTypeRef* constantNameAndType(size_t index){
        return (ConstantNameAndTypeRef*)constant(index);
    }

    FieldInfo* lookupField(string name, string descriptor){
        for(FieldInfo* field: fields){
            if(field->name == name && field->descriptor == descriptor){
                return field;
            }
        }
        return nullptr;
    }

    FieldInfo* lookupField(ConstantFieldRef* constantField){
        return lookupField(constantField->getNameAndType()->name->bytes,
                constantField->getNameAndType()->descriptor->bytes);
    }

    MethodInfo* lookupMethod(string name, string descriptor){
        for(MethodInfo* method: methods){
            if(method->name == name
               && method->descriptor == descriptor){
                return method;
            }
        }
        return nullptr;
    }

    MethodInfo* lookupMethod(MethodInfo* prototype){
        return lookupMethod(prototype->name, prototype->descriptor);
    }

    MethodInfo* lookupMethod(ConstantMethodRef* constantMethod){
        return lookupMethod(
                constantMethod->getNameAndType()->getName()->bytes,
                constantMethod->getNameAndType()->getDescriptor()->bytes);
    }
};

/*AttributeInfo* FieldInfo::findAttribute(Class* context, const char* name) {
    for(AttributeInfo* attribute: attributes){
        if(context->constant(attribute->nameIndex)->tag == ConstantType::UTF8){
            auto* utf8Info = (ConstantUtf8*)context->constant(attribute->nameIndex);
            if(utf8Info->stringEquals(name)){
                return attribute;
            }
        }
    }
    return nullptr;
}*/

}

#endif //TINYJ_CLASS_H
