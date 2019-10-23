//
// Created by chenjs on 2019/10/22.
//

#include "Object.h"
#include "ClassLoader.h"

namespace TinyJ{

bool ObjectHeader::isInstance(Class* targetKlass) {
    if((U16)targetKlass->accessFlags & (U16)AccessFlag::INTERFACE){
        for(U16 interface: klass->interfaceIndices){
            ConstantClass* interfaceRef = klass->constantClass(interface);
            ConstantUtf8* interfaceName = klass->constantString(interfaceRef->nameIndex);
            if(interfaceName->bytes == targetKlass->descriptor){
                return true;
            }
        }
        return false;
    }else {
        Class* pClass = klass;
        while(true){
            if(pClass == targetKlass){
                return true;
            }else if(pClass->superClassNameIndex == pClass->thisClassNameIndex){
                return false;
            }else{
                ConstantUtf8* className = pClass->constantString(pClass->constantClass(pClass->superClassNameIndex)->nameIndex);
                pClass = pClass->classLoader->lookupClassByDescriptor(className->bytes);
            }
        }
    }
}

}
