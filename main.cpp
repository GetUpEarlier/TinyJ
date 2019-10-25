#include <iostream>
#include "Vm.h"

using std::string;

int main() {
    using namespace TinyJ;
    string path = "/home/chenjs/Documents/CLionProjects/TinyJ/classes";
    auto* interpreter = new Interpreter(path);
    interpreter->invokeMain("Main2");
    return 0;
}