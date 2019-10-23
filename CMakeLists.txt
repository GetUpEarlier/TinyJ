cmake_minimum_required(VERSION 3.15)
project(TinyJ)

set(CMAKE_CXX_STANDARD 17)

add_executable(TinyJ main.cpp String.h Type.h Class.h Char.h Primitive.h Util.h ClassLoader.h InputStream.h Endian.h Value.h Vm.h Instruction.h ByteBuffer.h ClassLoader.cpp Class.cpp Object.h Object.cpp RuntimeClass.cpp RuntimeClass.h OperandStack.h Slot.h CodeReader.h StackFrame.h LocalTable.h Reference.h Heap.h Heap.cpp Array.h StaticTable.h Descriptor.h Descriptor.cpp)