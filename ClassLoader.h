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

    Class* lookupClassByDescriptor(string descriptor){
        if(_classes.count(descriptor) == 0){
            Class* klass = nullptr;
            if(descriptor.front() == '[') {
                klass = loadArrayClass(descriptor);
            }else if(descriptor.size() == 1 && parsePrimitiveFromDescriptor(descriptor).has_value()){
                klass = loadPrimitiveClass(parsePrimitiveFromDescriptor(descriptor).value());
            }else{
                descriptor.erase(0, 1);
                klass = loadObjectClass(descriptor);
            }
            _classes[descriptor] = klass;
        }
        return _classes[descriptor];
    }

    Class* loadPrimitiveClass(Primitive primitive){
        auto reference = _heap->alloc<Class>();
        auto* klass = _heap->deref<Class>(reference);
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
        Reference reference = _heap->alloc(sizeof(Class));
        auto* klass = _heap->deref<Class>(reference);
        klass->accessFlags = AccessFlag::PUBLIC;
        klass->elementClass = nullptr;
        klass->thisClass = klass;
        klass->superClass = lookupClassByDescriptor("Ljava/lang/Object");
        klass->interfaces.push_back(lookupClassByDescriptor("Ljava/lang/Cloneable"));
        klass->interfaces.push_back(lookupClassByDescriptor("Ljava/io/Serializable"));
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
        stream.seekg(0, std::ios::beg);
        BigEndian endian;
        InputStream in(&stream, &endian);
        Reference reference = _heap->alloc(sizeof(Class));
        auto* klass = _heap->deref<Class>(reference);
        klass->magic = in.readU32();
        klass->minorVersion = in.readU16();
        klass->majorVersion = in.readU16();
        U16 countConstantPool = in.readU16();
        for(U16 i = 0;i < countConstantPool-1;i++){
            auto tag = (ConstantType)(in.readU8());
            ConstantInfo* info = readConstantInfo(tag, &in);
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
        Reference staticTable = _heap->alloc(staticShift);
        klass->staticTable = _heap->deref<StaticTable>(staticTable);
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
        klass->descriptor = "L" + std::move(className);
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

    static AttributeInfo* readAttributeInfo(Class* klass, InputStream* in){
        U16 attributeName = in->readU16();
        U32 attributeLength = in->readU32();
        auto* name = (ConstantUtf8*)klass->constant(attributeName);
        AttributeInfo* attribute = nullptr;
        if(name->stringEquals("Code")){
            auto* attributeCode = new AttributeCode();
            attributeCode->maxStack = in->readU16();
            attributeCode->maxLocals = in->readU16();
            attributeCode->codeLength = in->readU32();
            attributeCode->code = new U8[attributeCode->codeLength];
            for(U32 i = 0;i < attributeCode->codeLength;i++){
                attributeCode->code[i] = in->readU8();
            }
            U16 exceptionTableLength = in->readU16();
            for(U16 i = 0;i < exceptionTableLength;i++){
                attributeCode->exceptionTable.push_back(readExceptionInfo(in));
            }
            U16 countAttributes = in->readU16();
            for(U16 i = 0;i < countAttributes;i++){
                attributeCode->attributes.push_back(readAttributeInfo(klass, in));
            }
            attribute = attributeCode;
        }else if(name->stringEquals("Exceptions")){
            auto* attributeExceptions = new AttributeExceptions();
            U16 numberOfExceptions = in->readU16();
            for(U16 i = 0;i < numberOfExceptions;i++){
                attributeExceptions->exceptionIndexTable.push_back(in->readU16());
            }
            attribute = attributeExceptions;
        }else if(name->stringEquals("LineNumberTable")){
            auto* attributeLineNumberTable = new AttributeLineNumberTable();
            U16 lineNumberTableLength = in->readU16();
            for(U16 i = 0;i < lineNumberTableLength;i++){
                attributeLineNumberTable->lineNumberTable.push_back(readLineNumberInfo(in));
            }
            attribute = attributeLineNumberTable;
        }else if(name->stringEquals("LocalVariableTable")){
            auto* attributeLocalVariableTable = new AttributeLocalVariableTable();
            U16 localVariableTableLength = in->readU16();
            for(U16 i = 0;i < localVariableTableLength;i++){
                attributeLocalVariableTable->localVariableTable.push_back(readLocalVariableInfo(in));
            }
            attribute = attributeLocalVariableTable;
        }else if(name->stringEquals("SourceFile")){
            auto* attributeSourceFile = new AttributeSourceFile();
            attributeSourceFile->sourceFileIndex = in->readU16();
            attribute = attributeSourceFile;
        }else if(name->stringEquals("ConstantValue")){
            auto* attributeConstantValue = new AttributeConstantValue();
            attributeConstantValue->constantValueIndex = in->readU16();
            attribute = attributeConstantValue;
        }else if(name->stringEquals("InnerClasses")){
            auto* attributeInnerClasses = new AttributeInnerClasses();
            U16 numberOfClasses = in->readU16();
            for(U16 i = 0;i < numberOfClasses;i++){
                attributeInnerClasses->innerClasses.push_back(readInnerClassesInfo(in));
            }
            attribute = attributeInnerClasses;
        }else if(name->stringEquals("Deprecated")){
            attribute = new AttributeDeprecated();
        }else if(name->stringEquals("Synthetic")){
            attribute = new AttributeSynthetic();
        }else if(name->stringEquals("Signatue")){
            auto* attributeSignature = new AttributeSignature();
            attributeSignature->signatureIndex = in->readU16();
            attribute = attributeSignature;
        }else if(name->stringEquals("BootstrapMethods")){
            auto* attributeBootstrapMethods = new AttributeBootstrapMethods();
            U16 numBootstrapMethods = in->readU16();
            for(U16 i = 0;i < numBootstrapMethods;i++){
                attributeBootstrapMethods->bootstrapMethods.push_back(readBootstrapMethod(in));
            }
            attribute = attributeBootstrapMethods;
        }else{
            auto* unknownAttribute = new UnknownAttribute();
            unknownAttribute->attributeLength = attributeLength;
            unknownAttribute->attributeContent = in->readN(attributeLength);
            attribute = unknownAttribute;
        }
        attribute->nameIndex = attributeName;
        attribute->klass = klass;
        attribute->name = name->bytes;
        return attribute;
    }

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

    static ConstantInfo* readConstantInfo(ConstantType type, InputStream* in){
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
                break;
            }
            case ConstantType::F64:{
                auto f64 = new ConstantDouble();
                *(U64*)&f64->value = in->readU64();
                info = f64;
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
