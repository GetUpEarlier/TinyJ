//
// Created by chenjs on 2019/10/22.
//

#include "ClassLoader.h"

#include "Value.h"

namespace TinyJ{

void ClassLoader::initClass(Class* klass, string descriptor) {
    klass->descriptor = std::move(descriptor);
//    klass->staticTable = new StaticTable();
    klass->classLoader = this;
    klass->superClass = klass->constantClass(klass->superClassNameIndex)
}

}