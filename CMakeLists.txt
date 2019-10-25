cmake_minimum_required(VERSION 3.15)
project(TinyJ)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)

pkg_check_modules(PKGS REQUIRED minizip)

include_directories(${PKGS_INCLUDE_DIRS})
link_directories(${PKGS_LIBRARY_DIRS})

add_executable(TinyJ main.cpp String.h Class.h Primitive.h ClassLoader.h InputStream.h Endian.h Value.h Vm.h Instruction.h ByteBuffer.h ClassLoader.cpp Class.cpp Object.h Object.cpp OperandStack.h Slot.h CodeReader.h StackFrame.h LocalTable.h Heap.h Heap.cpp Array.h StaticTable.h Descriptor.h Descriptor.cpp Number.h ClassPath.h Value.cpp)

target_link_libraries(TinyJ ${PKGS_LIBRARY_DIRS})