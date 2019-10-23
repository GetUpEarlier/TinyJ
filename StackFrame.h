//
// Created by chenjs on 2019/10/22.
//

#ifndef TINYJ_STACKFRAME_H
#define TINYJ_STACKFRAME_H

#include "OperandStack.h"
#include "LocalTable.h"

namespace TinyJ{

class StackFrame {
public:
//    explicit StackFrame(U32 maxLocals, U32 maxOperands): operandStack(maxOperands), localTable(maxLocals){}

    explicit StackFrame(AttributeCode* code)
        : operandStack(code->maxStack)
        , localTable(code->maxLocals)
        , code(code->code, code->codeLength)
        , context(code->klass){}

    OperandStack operandStack;
    LocalTable localTable;
    CodeReader code;
    Class* context;
};

}

#endif //TINYJ_STACKFRAME_H
