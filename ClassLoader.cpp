//
// Created by chenjs on 2019/10/22.
//

#include "ClassLoader.h"

#include "Value.h"

namespace TinyJ{


AttributeInfo* ClassLoader::readAttributeInfo(Class* klass, InputStream* in) {
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
}