//
// Created by chenjs on 2019/10/21.
//

#ifndef TINYJ_CLASSLOADER_H
#define TINYJ_CLASSLOADER_H

#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include "Class.h"
#include "Endian.h"
#include "InputStream.h"
#include "Value.h"
#include "Heap.h"

namespace TinyJ{

using std::basic_ifstream;
using std::ofstream;
using std::string;
using std::map;
using std::unordered_map;

class Interpreter;

class ClassLoader{
private:
    string _classPath;
    unordered_map<string, Class*> _classes;
    Heap* _heap;
public:
    explicit ClassLoader(string classPath, Heap* heap){
        _classPath = std::move(classPath);
        _heap = heap;
    }

    Class* lookupClassByName(string className){
        return lookupClassByDescriptor("L" + className + ";");
    }

    Class* lookupClassByDescriptor(string descriptor){
        if(_classes.count(descriptor) == 0){
            Class* klass = nullptr;
            if(descriptor.front() == '[') {
                klass = loadArrayClass(descriptor);
            }else if(descriptor.size() == 1 && parsePrimitiveFromDescriptor(descriptor).has_value()){
                klass = loadPrimitiveClass(parsePrimitiveFromDescriptor(descriptor).value());
            }else{
                string className = descriptor;
                className.pop_back();
                className.erase(0, 1);
                klass = loadObjectClass(className);
            }
            _classes.insert({descriptor, klass});
        }
        return _classes[descriptor];
    }

    Class* loadPrimitiveClass(Primitive primitive){
        auto* klass = allocClass();
        klass->isPrimitive = true;
        klass->size = primitiveSize(primitive);
        klass->primitive = primitive;
        klass->isArray = false;
        klass->staticTable = nullptr;
        klass->classLoader = this;
        klass->elementClass = nullptr;
        klass->descriptor = primitiveDescriptor(primitive);
        klass->thisClass = klass;
        klass->superClass = nullptr;
        return klass;
    }

    Class* loadArrayClass(string descriptor){
        auto* klass = allocClass();
        klass->accessFlags = AccessFlag::PUBLIC;
        klass->elementClass = nullptr;
        klass->thisClass = klass;
        klass->superClass = lookupClassByDescriptor("Ljava/lang/Object;");
        klass->interfaces.push_back(lookupClassByDescriptor("Ljava/lang/Cloneable;"));
        klass->interfaces.push_back(lookupClassByDescriptor("Ljava/io/Serializable;"));
        klass->isArray = true;
        klass->isPrimitive = false;
        klass->size = 32;
        klass->descriptor = std::move(descriptor);
        klass->staticTable = nullptr;
        klass->classLoader = this;
        return klass;
    }

    Class* loadObjectClass(string className){
        std::ifstream stream;
        stream.open(_classPath + "/" + className + ".class", std::ios::binary|std::ios::in);
        if(!stream.is_open()){
            throw std::exception();
        }
        stream.seekg(0, std::ios::beg);
        BigEndian endian;
        InputStream in(&stream, &endian);
        auto* klass = allocClass();
        klass->magic = in.readU32();
        klass->minorVersion = in.readU16();
        klass->majorVersion = in.readU16();
        U16 countConstantPool = in.readU16();
        bool skipNext = false;
        for(U16 i = 1;i < countConstantPool;i++){
            if(skipNext){
                skipNext = false;
                klass->constantPool.push_back(nullptr);
                continue;
            }
            auto tag = (ConstantType)(in.readU8());
            ConstantInfo* info = readConstantInfo(tag, &in, skipNext);
            info->klass = klass;
            klass->constantPool.push_back(info);
        }
        klass->accessFlags = (AccessFlag)in.readU16();
        klass->thisClassNameIndex = in.readU16();
        klass->superClassNameIndex = in.readU16();
        U16 countInterfaces = in.readU16();
        for(U16 i = 0;i < countInterfaces;i++){
            klass->interfaceIndices.push_back(in.readU16());
        }
        U16 countFields = in.readU16();
        U32 staticShift = 0;
        U32 instanceShift = 0;
        for(U16 i = 0;i < countFields;i++){
            FieldInfo* fieldInfo = readFieldInfo(klass, &in);
            if(fieldInfo->isStatic()){
                fieldInfo->shift = staticShift;
                staticShift += getSizeFromDescriptor(fieldInfo->descriptor);
            }else{
                fieldInfo->shift = instanceShift;
                instanceShift += getSizeFromDescriptor(fieldInfo->descriptor);
            }
            klass->fields.push_back(fieldInfo);
        }
        klass->objectSize = instanceShift;
        klass->staticTable = new StaticTable(staticShift);
        U16 countMethods = in.readU16();
        for(U16 i = 0;i < countMethods;i++){
            klass->methods.push_back(readMethodInfo(klass, &in));
        }
        U16 countAttributes = in.readU16();
        for(U16 i = 0;i < countAttributes;i++){
            klass->attributes.push_back(readAttributeInfo(klass, &in));
        }
        stream.close();
        klass->isArray = false;
        klass->isPrimitive = false;
        klass->size = 32;
        klass->descriptor = "L" + className;
        klass->classLoader = this;
        return klass;
    }

    Class* allocClass(){
        Reference reference = _heap->alloc(sizeof(Class));
        auto* klass = _heap->deref<Class>(reference);
        new (klass)Class;
        klass->status = ClassStatus::UN_INITED;
        klass->isPrimitive = false;
        klass->isArray = false;
        klass->thisClass = klass;
        klass->superClass = nullptr;
        return klass;
    }

    static MethodInfo* readMethodInfo(Class* klass, InputStream* in){
        auto* info = new MethodInfo();
        info->accessFlag = (MethodAccessFlag)in->readU16();
        info->nameIndex = in->readU16();
        info->descriptorIndex = in->readU16();
        U16 countAttributes = in->readU16();
        for(U16 i = 0;i < countAttributes;i++){
            AttributeInfo* attribute = readAttributeInfo(klass, in);
            info->attributes.push_back(attribute);
            if(attribute->name == "Code"){
                info->code = (AttributeCode*)attribute;
            }
        }
        info->klass = klass;
        info->name = klass->constantString(info->nameIndex)->bytes;
        info->descriptor = klass->constantString(info->descriptorIndex)->bytes;
        info->methodDescriptor = parseMethodDescriptor(info->descriptor);
        return info;
    }

    static FieldInfo* readFieldInfo(Class* klass, InputStream* in){
        auto* info = new FieldInfo();
        info->accessFlag = (FieldAccessFlag)in->readU16();
        info->nameIndex = in->readU16();
        info->descriptorIndex = in->readU16();
        U16 countAttributes = in->readU16();
        for(U16 i = 0;i < countAttributes;i++){
            AttributeInfo* attribute = readAttributeInfo(klass, in);
            info->attributes.push_back(attribute);
        }
        info->klass = klass;
        info->name = klass->constantString(info->nameIndex)->bytes;
        info->descriptor = klass->constantString(info->descriptorIndex)->bytes;
        info->size = getSizeFromDescriptor(info->descriptor);
        return info;
    }

    static AttributeInfo* readAttributeInfo(Class* klass, InputStream* in);

    static BootstrapMethod* readBootstrapMethod(InputStream* in){
        auto* bootstrapMethod = new BootstrapMethod();
        bootstrapMethod->bootstrapMethodRef = in->readU16();
        U16 numBootstrapArguments = in->readU16();
        for(U16 i = 0;i < numBootstrapArguments;i++){
            bootstrapMethod->bootstrapArguments.push_back(in->readU16());
        }
        return bootstrapMethod;
    }

    static InnerClassesInfo* readInnerClassesInfo(InputStream* in){
        auto* innerClassesInfo = new InnerClassesInfo();
        innerClassesInfo->innerClassInfoIndex = in->readU16();
        innerClassesInfo->outerClassInfoIndex = in->readU16();
        innerClassesInfo->innerNameIndex = in->readU16();
        innerClassesInfo->innerClassAccessFlag = (InnerClassAccessFlag)in->readU16();
        return innerClassesInfo;
    }

    static LocalVariableInfo* readLocalVariableInfo(InputStream* in){
        auto* localVariableInfo = new LocalVariableInfo();
        localVariableInfo->startPc = in->readU16();
        localVariableInfo->length = in->readU16();
        localVariableInfo->nameIndex = in->readU16();
        localVariableInfo->descriptorIndex = in->readU16();
        localVariableInfo->index = in->readU16();
        return localVariableInfo;
    }

    static LineNumberInfo* readLineNumberInfo(InputStream* in){
        auto* lineNumberInfo = new LineNumberInfo();
        lineNumberInfo->startPc = in->readU16();
        lineNumberInfo->lineNumber = in->readU16();
        return lineNumberInfo;
    }

    static ExceptionInfo* readExceptionInfo(InputStream* in){
        auto* exceptionInfo = new ExceptionInfo();
        exceptionInfo->startPc = in->readU16();
        exceptionInfo->endPc = in->readU16();
        exceptionInfo->handlerPc = in->readU16();
        exceptionInfo->catchType = in->readU16();
        return exceptionInfo;
    }

    static ConstantInfo* readConstantInfo(ConstantType type, InputStream* in, bool& skipNext){
        ConstantInfo* info = nullptr;
        switch(type){
            case ConstantType::UTF8:{
                auto* utf8 = new ConstantUtf8();
                U16 length = in->readU16();
                for(size_t j = 0;j < length;j++){
                    utf8->bytes.push_back(in->readU8());
                }
                info = utf8;
                break;
            }
            case ConstantType::I32:{
                auto i32 = new ConstantInteger();
                *(U32*)(&i32->value) = in->readU32();
                info = i32;
                break;
            }
            case ConstantType::F32:{
                auto f32 = new ConstantFloat();
                *(U32*)(&f32->value) = in->readU32();
                info = f32;
                break;
            }
            case ConstantType::I64:{
                auto i64 = new ConstantLong();
                *(U64*)&i64->value = in->readU64();
                info = i64;
                skipNext = true;
                break;
            }
            case ConstantType::F64:{
                auto f64 = new ConstantDouble();
                *(U64*)&f64->value = in->readU64();
                info = f64;
                skipNext = true;
                break;
            }
            case ConstantType::CLASS:{
                auto klass = new ConstantClass();
                klass->nameIndex = in->readU16();
                info = klass;
                break;
            }
            case ConstantType::STRING:{
                auto string = new ConstantString();
                string->utf8Index = in->readU16();
                string->reference = 0;
                string->utf8 = nullptr;
                info = string;
                break;
            }
            case ConstantType::FIELD_REF:{
                auto fieldRef = new ConstantFieldRef();
                fieldRef->classIndex = in->readU16();
                fieldRef->nameAndTypeIndex = in->readU16();
                info = fieldRef;
                break;
            }
            case ConstantType::METHOD_REF:{
                auto methodRef = new ConstantMethodRef();
                methodRef->classIndex = in->readU16();
                methodRef->nameAndTypeIndex = in->readU16();
                info = methodRef;
                break;
            }
            case ConstantType::INTERFACE_METHOD_REF:{
                auto methodRef = new ConstantInterfaceMethodRef();
                methodRef->classIndex = in->readU16();
                methodRef->nameAndTypeIndex = in->readU16();
                info = methodRef;
                break;
            }
            case ConstantType::NAME_AND_TYPE:{
                auto nameAndType = new ConstantNameAndTypeRef();
                nameAndType->nameIndex = in->readU16();
                nameAndType->descriptorIndex = in->readU16();
                info = nameAndType;
                break;
            }
            case ConstantType::METHOD_HANDLE:{
                auto methodHandle = new ConstantMethodHandle();
                methodHandle->referenceKind = in->readU8();
                methodHandle->referenceIndex = in->readU16();
                info = methodHandle;
                break;
            }
            case ConstantType::METHOD_TYPE:{
                auto methodType = new ConstantMethodType();
                methodType->descriptorIndex = in->readU16();
                info = methodType;
                break;
            }
            case ConstantType::INVOKE_DYNAMIC:{
                auto invokeDynamic = new ConstantInvokeDynamic();
                invokeDynamic->bootStrapMethodAttr = in->readU16();
                invokeDynamic->nameAndTypeIndex = in->readU16();
                info = invokeDynamic;
                break;
            }
            default:
                int i = 0;
                break;
        }
        info->tag = type;
        return info;
    }

    static bool equals(ConstantUtf8* lhs, ConstantUtf8* rhs){
        if(lhs->bytes.size() == rhs->bytes.size()){
            size_t size = lhs->bytes.size();
            for(size_t i = 0;i < size;i++){
                if(lhs->bytes[i] != rhs->bytes[i]){
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

}

#endif //TINYJ_CLASSLOADER_H
