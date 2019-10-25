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
        , code(code)
        , context(code->klass)
        , codeReader(code->code, code->codeLength, &pc)
        , pc(0){}

    OperandStack operandStack;
    LocalTable localTable;
    AttributeCode* code;
    CodeReader codeReader;
    Class* context;
    U32 pc;
};

}

#endif //TINYJ_STACKFRAME_H
