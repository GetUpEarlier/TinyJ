//
// Created by chenjs on 2019/10/22.
//
#include "Class.h"

#include "ClassLoader.h"
#include "Vm.h"

namespace TinyJ{

Class* ConstantClass::getClass() {
    if(klass == nullptr){
        klass = ClassMember::klass->classLoader->lookupClassByName(getName()->bytes);
    }
    return klass;
}

ConstantUtf8* ConstantClass::getName() {
    if(name == nullptr){
        name = ClassMember::klass->constantString(nameIndex);
    }
    return name;
}

ConstantUtf8* ConstantString::getUtf8() {
    if(utf8 == nullptr){
        utf8 = klass->constantString(utf8Index);
    }
    return utf8;
}

Reference ConstantString::getReference(Interpreter* interpreter) {
    if(reference == 0){
        reference = interpreter->internString(getUtf8()->bytes);
    }
    return reference;
}

ConstantUtf8* ConstantNameAndTypeRef::getName() {
    if(name == nullptr){
        name = klass->constantString(nameIndex);
    }
    return name;
}

ConstantUtf8* ConstantNameAndTypeRef::getDescriptor() {
    if(descriptor == nullptr){
        descriptor = klass->constantString(descriptorIndex);
    }
    return descriptor;
}

ConstantClass* ConstantFieldRef::getClass() {
    if(klass == nullptr){
        klass = ClassMember::klass->constantClass(classIndex);
    }
    return klass;
}

ConstantNameAndTypeRef* ConstantFieldRef::getNameAndType() {
    if(nameAndType == nullptr){
        nameAndType = ClassMember::klass->constantNameAndType(nameAndTypeIndex);
    }
    return nameAndType;
}

FieldInfo* ConstantFieldRef::getFieldInfo() {
    return getClass()->getClass()->lookupField(this);
}

ConstantClass* ConstantMethodRef::getClass() {
    if(klass == nullptr){
        klass = ClassMember::klass->constantClass(classIndex);
    }
    return klass;
}

ConstantNameAndTypeRef* ConstantMethodRef::getNameAndType() {
    if(nameAndType == nullptr){
        nameAndType = ClassMember::klass->constantNameAndType(nameAndTypeIndex);
    }
    return nameAndType;
}

MethodInfo* ConstantMethodRef::getMethodInfo() {
    return getClass()->getClass()->lookupMethod(this);
}

ConstantClass* ConstantInterfaceMethodRef::getClass() {
    if(klass == nullptr){
        klass = ClassMember::klass->constantClass(classIndex);
    }
    return klass;
}

ConstantNameAndTypeRef* ConstantInterfaceMethodRef::getNameAndType() {
    if(nameAndType == nullptr){
        nameAndType = ClassMember::klass->constantNameAndType(nameAndTypeIndex);
    }
    return nameAndType;
}

Class* FieldInfo::getClass() {
    if(klass == nullptr){
        klass = ClassMember::klass->classLoader->lookupClassByDescriptor(descriptor);
    }
    return klass;
}

Class* Class::getElementClass() {
    if(elementClass == nullptr){
        if(descriptor.front() == '['){
            string elementClassDescriptor = descriptor;
            elementClassDescriptor.erase(0, 1);
            elementClass = classLoader->lookupClassByDescriptor(elementClassDescriptor);
        }
    }
    return elementClass;
}

AttributeInfo* MethodInfo::findAttribute(Class* context, const char* name) {
    for(AttributeInfo* attribute: attributes){
        if(context->constant(attribute->nameIndex)->tag == ConstantType::UTF8){
            auto* utf8Info = (ConstantUtf8*)context->constant(attribute->nameIndex);
            if(utf8Info->stringEquals(name)){
                return attribute;
            }
        }
    }
    return nullptr;
}

}
