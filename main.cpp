#include <iostream>
#include <string>
#include "ClassLoader.h"
#include "Value.h"
#include "Vm.h"
#include "Slot.h"

using std::string;

int main() {
    using namespace TinyJ;
    string path = "/home/chenjs/Documents/Git/webserver/build/classes/java/main/com/chen1144/webserver/AsciiString.class";
    auto* loader = new ClassLoader("/home/chenjs/Documents/Git/webserver/build/classes/java/main");
    Class* file = loader->lookupClassByDescriptor("com/chen1144/webserver/AsciiString");
    std::cout << path.c_str() << std::endl;
    std::cout << "" << std::endl;
    std::cout << sizeof(Value) << std::endl;
}