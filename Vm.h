//
// Created by chenjs on 2019/10/22.
//

#ifndef TINYJ_VM_H
#define TINYJ_VM_H

#include <memory>
#include <vector>
#include <stack>
#include <cmath>
#include <map>
#include <unordered_map>
#include "Value.h"
#include "Class.h"
#include "Instruction.h"
//#include "Bytereader.h"
#include "Heap.h"
#include "Endian.h"
#include "Object.h"
#include "Array.h"
#include "CodeReader.h"
#include "OperandStack.h"
#include "StackFrame.h"

namespace TinyJ{

using std::vector;
using std::stack;

//using Frame = std::vector<Value>;

/*class OperandStack{
public:

};*/

struct Thread{
    vector<StackFrame*> frameStack;

    StackFrame* top(){
        return frameStack.back();
    }

    void push(StackFrame* frame){
        frameStack.push_back(frame);
    }

    StackFrame* pop(){
        StackFrame* top = frameStack.back();
        frameStack.pop_back();
        return top;
    }
};

class Interpreter{
private:
    ClassLoader* _classLoader;
    Heap* _heap;
public:
    explicit Interpreter(string classPath){
        _heap = new Heap(1024*1024*256);
        _classLoader = new ClassLoader(std::move(classPath), _heap);
    }

    void loop(MethodInfo* method){
        auto* thread = new Thread();
        thread->push(new StackFrame(method->code));
        while(true){
            StackFrame* frame = thread->top();
            CodeReader& reader = frame->code;
            OperandStack& opStack = frame->operandStack;
            LocalTable& locals = frame->localTable;
            Class* context = frame->context;
/*        }
    }

    void execute(Class* context, Thread* thread, MethodInfo* methodInfo){
        auto* codeAttribute = (AttributeCode*)methodInfo->findAttribute(context, "Code");
        U32 codeLength = codeAttribute->codeLength;
        U8* codeArray = codeAttribute->code;
        StackFrame* oldFrame = thread->top();
        CodeReader reader(codeArray, codeLength);
        thread->push(new StackFrame(codeAttribute->maxLocals, codeAttribute->maxStack));
        StackFrame* newFrame = thread->top();
        OperandStack& opStack = newFrame->operandStack;
        LocalTable& locals = newFrame->localTable;
        U32 argSlotCount = methodInfo->methodDescriptor->argSlotCount;
        for(U32 i = argSlotCount-1;i >= 0;i--){
            locals.at(i) = oldFrame->operandStack.pop();
        }
        while(true){*/
            auto code = reader.readOpCode();
            switch(code){
                case OpCode::I_LOAD:
                case OpCode::F_LOAD:
                case OpCode::A_LOAD:
                    opStack.push(locals.at(reader.readU8()));
                    break;
                case OpCode::D_LOAD:
                case OpCode::L_LOAD:
                    opStack.push2(locals.at2(reader.readU8()));
                    break;
                case OpCode::I_LOAD_0:
                case OpCode::F_LOAD_0:
                case OpCode::A_LOAD_0:
                    opStack.push(locals.at(0));
                    break;
                case OpCode::D_LOAD_0:
                case OpCode::L_LOAD_0:
                    opStack.push2(locals.at2(0));
                    break;
                case OpCode::I_LOAD_1:
                case OpCode::F_LOAD_1:
                case OpCode::A_LOAD_1:
                    opStack.push(locals.at(1));
                    break;
                case OpCode::D_LOAD_1:
                case OpCode::L_LOAD_1:
                    opStack.push2(locals.at2(1));
                    break;
                case OpCode::I_LOAD_2:
                case OpCode::A_LOAD_2:
                case OpCode::F_LOAD_2:
                    opStack.push(locals.at(2));
                    break;
                case OpCode::D_LOAD_2:
                case OpCode::L_LOAD_2:
                    opStack.push2(locals.at2(2));
                    break;
                case OpCode::I_LOAD_3:
                case OpCode::F_LOAD_3:
                case OpCode::A_LOAD_3:
                    opStack.push(locals.at(3));
                    break;
                case OpCode::D_LOAD_3:
                case OpCode::L_LOAD_3:
                    opStack.push2(locals.at2(3));
                    break;
                case OpCode::I_STORE:
                case OpCode::A_STORE:
                case OpCode::F_STORE:
                    locals.at(reader.readU8()) = opStack.top();
                    break;
                case OpCode::D_STORE:
                case OpCode::L_STORE:
                    locals.at2(reader.readU8()) = opStack.top2();
                    break;
                case OpCode::I_STORE_0:
                case OpCode::F_STORE_0:
                case OpCode::A_STORE_0:
                    locals.at(0) = opStack.pop();
                    break;
                case OpCode::D_STORE_0:
                case OpCode::L_STORE_0:
                    locals.at2(0) = opStack.pop2();
                    break;
                case OpCode::I_STORE_1:
                case OpCode::F_STORE_1:
                case OpCode::A_STORE_1:
                    locals.at(1) =  opStack.pop();
                    break;
                case OpCode::D_STORE_1:
                case OpCode::L_STORE_1:
                    locals.at2(1) =  opStack.pop2();
                    break;
                case OpCode::I_STORE_2:
                case OpCode::F_STORE_2:
                case OpCode::A_STORE_2:
                    locals.at(2) =  opStack.pop();
                    break;
                case OpCode::D_STORE_2:
                case OpCode::L_STORE_2:
                    locals.at2(2) =  opStack.pop2();
                    break;
                case OpCode::I_STORE_3:
                case OpCode::F_STORE_3:
                case OpCode::A_STORE_3:
                    locals.at(3) =  opStack.pop();
                    break;
                case OpCode::D_STORE_3:
                case OpCode::L_STORE_3:
                    locals.at2(3) =  opStack.pop2();
                    break;
                case OpCode::BI_PUSH:{
                     opStack.pushByte(reader.readByte());
                    break;
                }
                case OpCode::SI_PUSH:{
                     opStack.pushShort(reader.readShort());
                    break;
                }
                case OpCode::LDC:
                case OpCode::LDC_W:{
                    U16 index = 0;
                    if(code == OpCode::LDC){
                        index = reader.readU8();
                    }else {
                        index = reader.readU16();
                    }
                    ConstantInfo* constant = context->constant(index);
                    if(constant->tag == ConstantType::I32){
                        opStack.pushInt(((ConstantInteger*)constant)->value);
                    }else if(constant->tag == ConstantType::F32){
                        opStack.pushFloat(((ConstantInteger*)constant)->value);
                    }else if(constant->tag == ConstantType::STRING){
                        //TODO
                    }else if(constant->tag == ConstantType::CLASS){
                        //TODO
                    }else if(constant->tag == ConstantType::INTERFACE_METHOD_REF){
                        //TODO
                    }
                    break;
                }
                case OpCode::LDC2_W:{
                    U16 index = reader.readU16();
                    ConstantInfo* constant = context->constant(index);
                    if(constant->tag == ConstantType::I64){
                        opStack.pushLong(((ConstantLong*)constant)->value);
                    }else if(constant->tag == ConstantType::F64){
                        opStack.pushDouble(((ConstantDouble*)constant)->value);
                    }
                    break;
                }
                case OpCode::A_CONST_NULL:{
                     opStack.pushReference(0);
                    break;
                }
                case OpCode::I_CONST_M1:
                    opStack.pushInt(-1);
                    break;
                case OpCode::I_CONST_0:
                    opStack.pushInt(0);
                    break;
                case OpCode::I_CONST_1:
                    opStack.pushInt(1);
                    break;
                case OpCode::I_CONST_2:
                    opStack.pushInt(2);
                    break;
                case OpCode::I_CONST_3:
                    opStack.pushInt(3);
                    break;
                case OpCode::I_CONST_4:
                    opStack.pushInt(4);
                    break;
                case OpCode::I_CONST_5:
                    opStack.pushInt(5);
                    break;
                case OpCode::L_CONST_0:
                    opStack.pushLong(0);
                    break;
                case OpCode::L_CONST_1:
                    opStack.pushInt(1);
                    break;
                case OpCode::F_CONST_0:
                    opStack.pushFloat(0);
                    break;
                case OpCode::F_CONST_1:
                    opStack.pushFloat(1);
                    break;
                case OpCode::F_CONST_2:
                    opStack.pushFloat(2);
                    break;
                case OpCode::D_CONST_0:
                    opStack.pushDouble(0);
                    break;
                case OpCode::D_CONST_1:
                    opStack.pushDouble(1);
                    break;
                case OpCode::WIDE:{
                    auto nextCode = (OpCode)reader.readU8();
                    switch (nextCode){
                        case OpCode::I_LOAD:
                        case OpCode::L_LOAD:
                        case OpCode::F_LOAD:
                        case OpCode::D_LOAD:
                        case OpCode::A_LOAD:{
                             opStack.push(locals.at(reader.readU16()));
                            break;
                        }
                        case OpCode::I_STORE:
                        case OpCode::L_STORE:
                        case OpCode::F_STORE:
                        case OpCode::D_STORE:
                        case OpCode::A_STORE:{
                            locals.at(reader.readU16()) = opStack.pop();
                            break;
                        }
                        case OpCode::I_INC:{
                            U16 index = reader.readU16();
                            Slot& value = locals.at(index);
                            Short inc = reader.readShort();
                            value.intValue += inc;
                            break;
                        }
                        default:{
                            //TODO
                        }
                    }
                    break;
                }
                case OpCode::I_ADD:{
                    Int value2 = opStack.pop().intValue;
                    Int value1 = opStack.pop().intValue;
                    opStack.pushInt(value1+value2);
                    break;
                }
                case OpCode::F_ADD:{
                    Float value2 = opStack.pop().floatValue;
                    Float value1 = opStack.pop().floatValue;
                    opStack.pushFloat(value1+value2);
                    break;
                }
                case OpCode::L_ADD:{
                    Long value2 = opStack.pop2().longValue;
                    Long value1 = opStack.pop2().longValue;
                    opStack.pushLong(value1+value2);
                    break;
                }
                case OpCode::D_ADD:{
                    Double value2 = opStack.pop2().doubleValue;
                    Double value1 = opStack.pop2().doubleValue;
                    opStack.pushDouble(value1+value2);
                    break;
                }
                case OpCode::I_SUB:{
                    Int value2 = opStack.pop().intValue;
                    Int value1 = opStack.pop().intValue;
                    opStack.pushInt(value1-value2);
                    break;
                }
                case OpCode::F_SUB:{
                    Float value2 = opStack.pop().floatValue;
                    Float value1 = opStack.pop().floatValue;
                    opStack.pushFloat(value1-value2);
                    break;
                }
                case OpCode::L_SUB:{
                    Long value2 = opStack.pop2().longValue;
                    Long value1 = opStack.pop2().longValue;
                    opStack.pushLong(value1-value2);
                    break;
                }
                case OpCode::D_SUB:{
                    Double value2 = opStack.pop2().doubleValue;
                    Double value1 = opStack.pop2().doubleValue;
                    opStack.pushDouble(value1-value2);
                    break;
                }
                case OpCode::I_MUL:{
                    Int value2 = opStack.pop().intValue;
                    Int value1 = opStack.pop().intValue;
                    opStack.pushInt(value1*value2);
                    break;
                }
                case OpCode::F_MUL:{
                    Float value2 = opStack.pop().floatValue;
                    Float value1 = opStack.pop().floatValue;
                    opStack.pushFloat(value1*value2);
                    break;
                }
                case OpCode::L_MUL:{
                    Long value2 = opStack.pop2().longValue;
                    Long value1 = opStack.pop2().longValue;
                    opStack.pushLong(value1*value2);
                    break;
                }
                case OpCode::D_MUL:{
                    Double value2 = opStack.pop2().doubleValue;
                    Double value1 = opStack.pop2().doubleValue;
                    opStack.pushDouble(value1*value2);
                    break;
                }
                case OpCode::I_DIV:{
                    Int value2 = opStack.pop().intValue;
                    Int value1 = opStack.pop().intValue;
                    opStack.pushInt(value1/value2);
                    break;
                }
                case OpCode::F_DIV:{
                    Float value2 = opStack.pop().floatValue;
                    Float value1 = opStack.pop().floatValue;
                    opStack.pushFloat(value1/value2);
                    break;
                }
                case OpCode::L_DIV:{
                    Long value2 = opStack.pop2().longValue;
                    Long value1 = opStack.pop2().longValue;
                    opStack.pushLong(value1/value2);
                    break;
                }
                case OpCode::D_DIV:{
                    Double value2 = opStack.pop2().doubleValue;
                    Double value1 = opStack.pop2().doubleValue;
                    opStack.pushDouble(value1/value2);
                    break;
                }
                case OpCode::I_REM:{
                    Int value2 = opStack.pop().intValue;
                    Int value1 = opStack.pop().intValue;
                    opStack.pushInt(value1%value2);
                    break;
                }
                case OpCode::F_REM:{
                    Float value2 = opStack.pop().floatValue;
                    Float value1 = opStack.pop().floatValue;
                    opStack.pushFloat(fmodf32(value1, value2));
                    break;
                }
                case OpCode::L_REM:{
                    Long value2 = opStack.pop2().longValue;
                    Long value1 = opStack.pop2().longValue;
                    opStack.pushLong(value1%value2);
                    break;
                }
                case OpCode::D_REM:{
                    Double value2 = opStack.pop2().doubleValue;
                    Double value1 = opStack.pop2().doubleValue;
                    opStack.pushDouble(fmodf64(value1, value2));
                    break;
                }
                case OpCode::I_NEG:{
                    opStack.pushInt(-opStack.pop().intValue);
                    break;
                }
                case OpCode::L_NEG:{
                    opStack.pushLong(-opStack.pop2().longValue);
                    break;
                }
                case OpCode::F_NEG:{
                    opStack.pushFloat(-opStack.pop().floatValue);
                    break;
                }
                case OpCode::D_NEG:{
                    opStack.pushDouble(-opStack.pop2().doubleValue);
                    break;
                }
                case OpCode::I_SHL:{
                    U32 value2 = opStack.pop().intValue;
                    U32 value1 = (U32)opStack.pop().intValue;
                    auto s = (U32)value2 & 0b11111u;
                    opStack.pushInt((Int)(value1 << s));
                    break;
                }
                case OpCode::I_SHR:{
                    U32 value2 = opStack.pop().intValue;
                    Int value1 = opStack.pop().intValue;
                    auto s = (U32)value2 & 0b11111u;
                    opStack.pushInt((Int)(value1 >> s));
                    break;
                }
                case OpCode::I_U_SHR:{
                    U32 value2 = opStack.pop().intValue;
                    U32 value1 = (U32)opStack.pop().intValue;
                    auto s = (U32)value2 & 0b11111u;
                    opStack.pushInt((Int)(value1 >> s));
                    break;
                }
                case OpCode::L_SHL:{
                    U32 value2 = opStack.pop().intValue;
                    U64 value1 = (U64)opStack.pop2().longValue;
                    auto s = value2 & 0b11111u;
                    opStack.pushLong((Long)(value1 << s));
                    break;
                }
                case OpCode::L_SHR:{
                    U32 value2 = opStack.pop().intValue;
                    Long value1 = opStack.pop2().longValue;
                    auto s = value2 & 0b11111u;
                    opStack.pushLong((Long)(value1 >> s));
                    break;
                }
                case OpCode::L_U_SHR:{
                    U32 value2 = opStack.pop().intValue;
                    U64 value1 = (U64)opStack.pop2().longValue;
                    auto s = value2 & 0b11111u;
                    opStack.pushLong((Long)(value1 >> s));
                    break;
                }
                case OpCode::I_OR:{
                    U32 value2 = opStack.pop().intValue;
                    U32 value1 = opStack.pop().intValue;
                    opStack.pushInt(value1|value2);
                    break;
                }
                case OpCode::L_OR:{
                    U64 value2 = opStack.pop2().longValue;
                    U64 value1 = opStack.pop2().longValue;
                    opStack.pushLong(value1|value2);
                    break;
                }
                case OpCode::I_AND:{
                    U32 value2 = opStack.pop().intValue;
                    U32 value1 = opStack.pop().intValue;
                    opStack.pushInt(value1&value2);
                    break;
                }
                case OpCode::L_AND:{
                    U64 value2 = opStack.pop2().longValue;
                    U64 value1 = opStack.pop2().longValue;
                    opStack.pushLong(value1&value2);
                    break;
                }
                case OpCode::I_XOR:{
                    U32 value2 = opStack.pop().intValue;
                    U32 value1 = opStack.pop().intValue;
                    opStack.pushInt(value1^value2);
                    break;
                }
                case OpCode::L_XOR:{
                    U64 value2 = opStack.pop2().longValue;
                    U64 value1 = opStack.pop2().longValue;
                    opStack.pushLong(value1^value2);
                    break;
                }
                case OpCode::I_INC:{
                    U8 index = reader.readU8();
                    Int value = opStack.pop().intValue;
                    Byte inc = reader.readU8();
                    opStack.pushInt(value + inc);
                    break;
                }
                case OpCode::D_CMP_G:
                case OpCode::D_CMP_L:{
                    Double value2 = opStack.pop2().doubleValue;
                    Double value1 = opStack.pop2().doubleValue;
                    if(value1 > value2){
                         opStack.pushInt(1);
                    }else if(value1 == value2){
                        opStack.pushInt(0);
                    }else if(value1 < value2){
                        opStack.pushInt(-1);
                    }else{
                        if(code == OpCode::D_CMP_G){
                            opStack.pushInt(1);
                        }else{
                            opStack.pushInt(-1);
                        }
                    }
                    break;
                }
                case OpCode::F_CMP_G:
                case OpCode::F_CMP_L:{
                    Float value2 = opStack.pop().floatValue;
                    Float value1 = opStack.pop().floatValue;
                    if(value1 > value2){
                        opStack.pushInt(1);
                    }else if(value1 == value2){
                        opStack.pushInt(0);
                    }else if(value1 < value2){
                        opStack.pushInt(-1);
                    }else{
                        if(code == OpCode::F_CMP_G){
                            opStack.pushInt(1);
                        }else{
                            opStack.pushInt(-1);
                        }
                    }
                    break;
                }
                case OpCode::L_CMP:{
                    Long value2 = opStack.pop2().longValue;
                    Long value1 = opStack.pop2().longValue;
                    if(value1 > value2){
                        opStack.pushInt(1);
                    }else if(value1 == value2){
                        opStack.pushInt(0);
                    }else if(value1 < value2){
                        opStack.pushInt(-1);
                    }else{
                        throw std::exception();
                    }
                    break;
                }
                case OpCode::I2L:{
                    opStack.pushLong(opStack.pop().intValue);
                    break;
                }
                case OpCode::I2F:{
                    opStack.pushFloat(opStack.pop().intValue);
                    break;
                }
                case OpCode::I2D:{
                    opStack.pushDouble(opStack.pop().intValue);
                    break;
                }
                case OpCode::L2F:{
                    opStack.pushFloat(opStack.pop2().longValue);
                    break;
                }
                case OpCode::L2D:{
                    opStack.pushDouble(opStack.pop2().longValue);
                    break;
                }
                case OpCode::F2D:{
                    opStack.pushDouble(opStack.pop().floatValue);
                    break;
                }
                case OpCode::I2B:{
                    opStack.pushByte((Byte)opStack.pop().intValue);
                }
                case OpCode::I2C:{
                    opStack.pushChar((Char)opStack.pop().intValue);
                    break;
                }
                case OpCode::I2S:{
                    opStack.pushShort((Short)opStack.pop().intValue);
                    break;
                }
                case OpCode::L2I:{
                    opStack.pushInt((Int)opStack.pop2().longValue);
                    break;
                }
                case OpCode::F2I:{
                    opStack.pushInt((Int)opStack.pop().floatValue);
                    break;
                }
                case OpCode::F2L:{
                    opStack.pushLong((Long)opStack.pop().floatValue);
                    break;
                }
                case OpCode::D2I:{
                    opStack.pushInt((Int)opStack.pop2().doubleValue);
                    break;
                }
                case OpCode::D2L:{
                    opStack.pushLong((Long)opStack.pop2().doubleValue);
                    break;
                }
                case OpCode::D2F:{
                    opStack.pushFloat((Float)opStack.pop2().doubleValue);
                    break;
                }
                case OpCode::NEW:{
                    U16 index = reader.readU16();
                    auto classInfo = (ConstantClass*)context->constant(index);
                    Reference reference = newObject(classInfo->getClass());
                    opStack.pushReference(reference);
                    break;
                }
                case OpCode::NEW_ARRAY:{
                    auto which = (Primitive)reader.readU8();
                    U32 length = (U32)opStack.pop().intValue;
                    opStack.pushReference(newPrimitiveArray(which, length));
                    break;
                }
                case OpCode::A_NEW_ARRAY:{
                    U16 classIndex = reader.readU16();
                    auto* className = context->constantString(context->constantClass(classIndex)->nameIndex);
                    auto* klass = _classLoader->lookupClassByDescriptor(className->bytes);
                    Int length = opStack.pop().intValue;
                    Reference array = newArray("L"+context->constantClass(classIndex)->getName()->bytes, length);
                    opStack.pushReference(array);
                    break;
                }
                case OpCode::MULTI_A_NEW_ARRAY:{
                    U16 classIndex = reader.readU16();
                    U8 dimensions = reader.readU8();
                    auto* className = context->constantClass(classIndex)->getName();
                    vector<Int> lengths;
                    for(U8 i = 0;i < dimensions;i++){
                        lengths.push_back((U32)opStack.pop().intValue);
                    }
                    Reference reference = newMultiDimensionArray(className->bytes, lengths);
                    opStack.pushReference(reference);
                    break;
                }
                case OpCode::ARRAY_LENGTH:{
                    Reference array = opStack.pop().referenceValue;
                    opStack.pushInt(_heap->deref<ArrayHeader>(array)->length);
                    break;
                }
                case OpCode::B_A_LOAD:
                case OpCode::C_A_LOAD:
                case OpCode::S_A_LOAD:
                case OpCode::I_A_LOAD:
                case OpCode::F_A_LOAD:
                case OpCode::A_A_LOAD:{
                    Reference arrayReference = opStack.pop().referenceValue;
                    auto* array = _heap->deref<ArrayHeader>(arrayReference);
                    Int index = opStack.pop().intValue;
                    opStack.push(array->slotAt(index));
                    break;
                }
                case OpCode::L_A_LOAD:
                case OpCode::D_A_LOAD:{
                    Reference arrayReference = opStack.pop().referenceValue;
                    auto* array = _heap->deref<ArrayHeader>(arrayReference);
                    Int index = opStack.pop().intValue;
                    opStack.push2(array->slot2At(index));
                    break;
                }
                case OpCode::B_A_STORE:
                case OpCode::C_A_STORE:
                case OpCode::S_A_STORE:
                case OpCode::I_A_STORE:
                case OpCode::F_A_STORE:
                case OpCode::A_A_STORE:{
                    Reference arrayReference = opStack.pop().referenceValue;
                    Int index = opStack.pop().intValue;
                    Slot value = opStack.pop();
                    _heap->deref<ArrayHeader>(arrayReference)->slotAt(index) = value;
                    break;
                }
                case OpCode::L_A_STORE:
                case OpCode::D_A_STORE:{
                    Reference arrayReference = opStack.pop().referenceValue;
                    Int index = opStack.pop().intValue;
                    Slot2 value = opStack.pop2();
                    _heap->deref<ArrayHeader>(arrayReference)->slot2At(index) = value;
                    break;
                }
                case OpCode::GET_FIELD:{
                    U16 index = reader.readU16();
                    auto* object = _heap->deref<ObjectHeader>(opStack.pop().referenceValue);
                    FieldInfo* fieldInfo = context->constantField(index)->getFieldInfo();
                    if(fieldInfo->size == 32){
                        opStack.push(object->slotAt(fieldInfo->shift));
                    }else if(fieldInfo->size == 64){
                        opStack.push2(object->slot2At(fieldInfo->shift));
                    }else{
                        throw std::exception();
                    }
                    break;
                }
                case OpCode::PUT_FIELD:{
                    U16 index = reader.readU16();
                    auto* object = _heap->deref<ObjectHeader>(opStack.pop().referenceValue);
                    FieldInfo* fieldInfo = context->constantField(index)->getFieldInfo();
                    if(fieldInfo->size == 32){
                        object->slotAt(fieldInfo->shift) = opStack.pop();
                    }else if(fieldInfo->size == 64){
                        object->slot2At(fieldInfo->shift) = opStack.pop2();
                    }else{
                        throw std::exception();
                    }
                    break;
                }
                case OpCode::GET_STATIC:{
                    U16 index = reader.readU16();
                    auto* constantField = context->constantField(index);
                    auto* fieldName = constantField->getNameAndType()->getName();
                    Class* klass = constantField->klass->getClass();
                    FieldInfo* fieldInfo = constantField->getFieldInfo();
                    if(fieldInfo->size == 32){
                        opStack.push(klass->staticTable->at(fieldInfo->shift));
                    }else if(fieldInfo->size == 64){
                        opStack.push2(klass->staticTable->at2(fieldInfo->shift));
                    }else{
                        throw std::exception();
                    }
                    break;
                }
                case OpCode::PUT_STATIC:{
                    U16 index = reader.readU16();
                    auto* constantField = context->constantField(index);
                    auto* fieldName = constantField->getNameAndType()->getName();
                    Class* klass = constantField->klass->getClass();
                    FieldInfo* fieldInfo = constantField->getFieldInfo();
                    if(fieldInfo->size == 32){
                        klass->staticTable->at(fieldInfo->shift) = opStack.pop();
                    }else if(fieldInfo->size == 64){
                        klass->staticTable->at2(fieldInfo->shift) = opStack.pop2();
                    }else{
                        throw std::exception();
                    }
                    break;
                }
                case OpCode::INSTANCE_OF:{
                    auto* objectRef = _heap->deref<ObjectHeader>(opStack.pop().referenceValue);
                    U16 classIndex = reader.readU16();
                    ConstantClass* classInfo = context->constantClass(classIndex);
                    Class* klass = classInfo->getClass();
                    if(objectRef->isInstance(klass)){
                         opStack.pushInt(1);
                    }else{
                         opStack.pushInt(0);
                    }
                    break;
                }
                case OpCode::CHECK_CAST:{
                    auto* objectRef = _heap->deref<ObjectHeader>(opStack.pop().referenceValue);
                    U16 classIndex = reader.readU16();
                    ConstantClass* classInfo = context->constantClass(classIndex);
                    Class* klass = classInfo->getClass();
                    if(objectRef->isInstance(klass)){
                        //TODO: throw Java Exception
                    }
                    break;
                }
                case OpCode::POP:{
                    opStack.pop();
                    break;
                }
                case OpCode::POP2:{
                    opStack.pop2();
                    break;
                }
                case OpCode::DUP:{
                    Slot value = opStack.pop();
                    opStack.push(value);
                    opStack.push(value);
                    break;
                }
                case OpCode::DUP_X1:{
                    Slot value1 = opStack.pop();
                    Slot value2 = opStack.pop();
                    opStack.push(value1);
                    opStack.push(value2);
                    opStack.push(value1);
                    break;
                }
                case OpCode::DUP_X2:{
                    Slot value1 = opStack.pop();
                    Slot value2 = opStack.pop();
                    Slot value3 = opStack.pop();
                    opStack.push(value1);
                    opStack.push(value3);
                    opStack.push(value2);
                    opStack.push(value1);
                    break;
                }
                case OpCode::DUP2:{
                    Slot value1 = opStack.pop();
                    Slot value2 = opStack.pop();
                    opStack.push(value2);
                    opStack.push(value1);
                    opStack.push(value2);
                    opStack.push(value1);
                    break;
                }
                case OpCode::DUP2_X1:{
                    Slot value1 = opStack.pop();
                    Slot value2 = opStack.pop();
                    Slot value3 = opStack.pop();
                    opStack.push(value2);
                    opStack.push(value1);
                    opStack.push(value3);
                    opStack.push(value2);
                    opStack.push(value1);
                    break;
                }
                case OpCode::DUP2_X2:{
                    Slot value1 = opStack.pop();
                    Slot value2 = opStack.pop();
                    Slot value3 = opStack.pop();
                    Slot value4 = opStack.pop();
                    opStack.push(value2);
                    opStack.push(value1);
                    opStack.push(value4);
                    opStack.push(value3);
                    opStack.push(value2);
                    opStack.push(value1);
                    break;
                }
                case OpCode::SWAP:{
                    Slot value1 = opStack.pop();
                    Slot value2 = opStack.pop();
                    opStack.push(value1);
                    opStack.push(value2);
                    break;
                }
                case OpCode::IF_A_CMP_EQ:
                case OpCode::IF_A_CMP_NE:{
                    Short branch = reader.readShort();
                    Reference value2 = opStack.pop().referenceValue;
                    Reference value1 = opStack.pop().referenceValue;
                    if(value1 == value2 && code == OpCode::IF_A_CMP_EQ){
                        reader.incPc(branch-1);
                    }else if(value1 != value2 && code == OpCode::IF_A_CMP_NE){
                        reader.incPc(branch-1);
                    }
                    break;
                }
                case OpCode::IF_I_CMP_EQ:
                case OpCode::IF_I_CMP_NE:
                case OpCode::IF_I_CMP_LT:
                case OpCode::IF_I_CMP_GE:
                case OpCode::IF_I_CMP_GT:
                case OpCode::IF_I_CMP_LE:{
                    Short branch = reader.readShort();
                    Int value2 = opStack.pop().intValue;
                    Int value1 = opStack.pop().intValue;
                    if(value1 == value2 && code == OpCode::IF_I_CMP_EQ){
                        reader.incPc(branch-1);
                    }else if(value1 != value2 && code == OpCode::IF_I_CMP_NE){
                        reader.incPc(branch-1);
                    }else if(value1 < value2 && code == OpCode::IF_I_CMP_LT){
                        reader.incPc(branch-1);
                    }else if(value1 <= value2 && code == OpCode::IF_I_CMP_LE){
                        reader.incPc(branch-1);
                    }else if(value1 > value2 && code == OpCode::IF_I_CMP_GT){
                        reader.incPc(branch-1);
                    }else if(value1 >= value2 && code == OpCode::IF_I_CMP_GE){
                        reader.incPc(branch-1);
                    }
                    break;
                }
                case OpCode::IF_EQ:
                case OpCode::IF_NE:
                case OpCode::IF_LT:
                case OpCode::IF_GE:
                case OpCode::IF_GT:
                case OpCode::IF_LE:{
                    Short branch = reader.readShort();
                    Int value = opStack.pop().intValue;
                    if(value == 0 && code == OpCode::IF_EQ){
                        reader.incPc(branch-1);
                    }else if(value != 0 && code == OpCode::IF_NE){
                        reader.incPc(branch-1);
                    }else if(value < 0 && code == OpCode::IF_LT){
                        reader.incPc(branch-1);
                    }else if(value <= 0 && code == OpCode::IF_LE){
                        reader.incPc(branch-1);
                    }else if(value > 0 && code == OpCode::IF_GT){
                        reader.incPc(branch-1);
                    }else if(value >= 0 && code == OpCode::IF_GE){
                        reader.incPc(branch-1);
                    }
                    break;
                }
                case OpCode::IF_NON_NULL:
                case OpCode::IF_NULL:{
                    Short branch = reader.readShort();
                    Reference value = opStack.pop().referenceValue;
                    if(value != 0 && code == OpCode::IF_NULL){
                        reader.incPc(branch-1);
                    }else if(value == 0 && code == OpCode::IF_NON_NULL){
                        reader.incPc(branch-1);
                    }
                    break;
                }
                case OpCode::TABLE_SWITCH:{
                    Int thisAddress = reader.pc() - 1;
                    while(reader.pc() % 4 != 0){
                        reader.readU8();
                    }
                    Int defaultValue = reader.readInt();
                    Int lowValue = reader.readInt();
                    Int highValue = reader.readInt();
                    Int index = opStack.pop().intValue;
                    if(index < lowValue || index > highValue){
                        reader.incPc(defaultValue);
                    }else{
                        reader.incPc(index - lowValue);
                        Int offset = reader.readInt();
                        reader.setPc(thisAddress + offset);
                    }
                    break;
                }
                case OpCode::LOOKUP_SWITCH:{
                    Int thisAddress = reader.pc() - 1;
                    while(reader.pc() % 4 != 0){
                        reader.readU8();
                    }
                    Int defaultValue = reader.readInt();
                    Int nPairs = reader.readInt();
                    Int key = opStack.pop().intValue;
                    Int offset = defaultValue;
                    for(Int i = 0;i < nPairs;i++){
                        Int pairKey = reader.readInt();
                        Int pairValue = reader.readInt();
                        if(pairKey == key){
                            offset = pairValue;
                            break;
                        }
                    }
                    reader.setPc(thisAddress+offset);
                    break;
                }
                case OpCode::GOTO:{
                    Short branch = reader.readShort();
                    reader.incPc(branch-1);
                    break;
                }
                case OpCode::GOTO_W:{
                    Int branch = reader.readInt();
                    reader.incPc(branch-1);
                    break;
                }
                case OpCode::JSR:
                case OpCode::JSR_W:
                case OpCode::RET:
                case OpCode::RET_W:{
                    throw std::exception();
                }
                case OpCode::INVOKE_STATIC:{
                    U16 index = reader.readU16();
                    ConstantMethodRef* methodRef = context->constantMethod(index);
                    MethodInfo* methodToInvoke = methodRef->getMethodInfo();
                    if(!methodToInvoke->isStatic()){
                        throw std::exception();
                    }
                    U32 argSlotCount = methodToInvoke->methodDescriptor->argSlotCount;
                    auto* newFrame = new StackFrame(methodToInvoke->code);
                    for(U32 i = argSlotCount-1;i >= 0;i--){
                        newFrame->localTable.at(i) = opStack.pop();
                    }
                    thread->push(newFrame);
                    break;
                }
                case OpCode::INVOKE_SPECIAL:{
                    U16 index = reader.readU16();
                    ConstantMethodRef* methodRef = context->constantMethod(index);
                    MethodInfo* methodToInvoke = methodRef->getMethodInfo();
                    if(methodToInvoke->isStatic()){
                        throw std::exception();
                    }
                    U32 argSlotCount = methodToInvoke->methodDescriptor->argSlotCount;
                    argSlotCount += 1;
                    auto* newFrame = new StackFrame(methodToInvoke->code);
                    for(U32 i = argSlotCount-1;i >= 0;i--){
                        newFrame->localTable.at(i) = opStack.pop();
                    }
                    thread->push(newFrame);
                    break;
                }
                case OpCode::INVOKE_VIRTUAL:{
                    U16 index = reader.readU16();
                    ConstantMethodRef* methodRef = context->constantMethod(index);
                    MethodInfo* methodToInvoke = methodRef->getMethodInfo();
                    if(methodToInvoke->isStatic()){
                        throw std::exception();
                    }
                    U32 argSlotCount = methodToInvoke->methodDescriptor->argSlotCount;
                    argSlotCount += 1;
                    auto* newFrame = new StackFrame(methodToInvoke->code);
                    for(U32 i = argSlotCount-1;i >= 0;i--){
                        newFrame->localTable.at(i) = opStack.pop();
                    }
                    Reference reference = newFrame->localTable.at(0).referenceValue;
                    auto* targetObject = _heap->deref<ObjectHeader>(reference);
                    Class* targetClass = targetObject->klass;
                    MethodInfo* realMethod = targetClass->lookupMethod(
                            methodToInvoke->name,
                            methodToInvoke->descriptor);
                    newFrame->code = CodeReader(realMethod->code->code, realMethod->code->codeLength);
                    newFrame->context = realMethod->klass;
                    thread->push(newFrame);
                    break;
                }
                case OpCode::INVOKE_INTERFACE:{
                    U16 index = reader.readU16();
                    U16 unused = reader.readU16();
                    ConstantMethodRef* methodRef = context->constantMethod(index);
                    MethodInfo* methodToInvoke = methodRef->getMethodInfo();
                    if(methodToInvoke->isStatic()){
                        throw std::exception();
                    }
                    U32 argSlotCount = methodToInvoke->methodDescriptor->argSlotCount;
                    argSlotCount += 1;
                    auto* newFrame = new StackFrame(methodToInvoke->code);
                    for(U32 i = argSlotCount-1;i >= 0;i--){
                        newFrame->localTable.at(i) = opStack.pop();
                    }
                    Reference reference = newFrame->localTable.at(0).referenceValue;
                    auto* targetObject = _heap->deref<ObjectHeader>(reference);
                    Class* targetClass = targetObject->klass;
                    MethodInfo* realMethod = targetClass->lookupMethod(
                            methodToInvoke->name,
                            methodToInvoke->descriptor);
                    newFrame->code = CodeReader(realMethod->code->code, realMethod->code->codeLength);
                    newFrame->context = realMethod->klass;
                    thread->push(newFrame);
                    break;
                }
            }
            break;
        }
    }

    Reference newObject(Class* klass){
        U32 size = sizeof(ObjectHeader) + klass->objectSize;
        Reference reference = _heap->alloc(size);
        ((ObjectHeader*)_heap)->klass = klass;
        return reference;
    }

    Reference newArray(string descriptor, Int length){
        if(length < 0){
            return 0;
        }
        U32 size = sizeof(ArrayHeader) + length*getSizeFromDescriptor(descriptor);
        Reference reference = _heap->alloc(size);
        string arrayClassName = "[" + std::move(descriptor);
        auto* header = (ArrayHeader*)_heap->deref(reference);
        header->klass = _classLoader->lookupClassByDescriptor(arrayClassName);
        header->length = length;
        return reference;
    }

    Reference newPrimitiveArray(Primitive primitive, Int length){
        return newArray(primitiveDescriptor(primitive), length);
    }

    Reference newMultiDimensionArray(string descriptor, vector<Int> dimensions){
        if(dimensions.empty()){
            return 0;
        }else if(dimensions.size() == 1){
            return newArray(std::move(descriptor), dimensions.front());
        }else{
            string arrayDescriptor = descriptor;
            for(size_t i = 1;i < dimensions.size();i++){
                arrayDescriptor.insert(0, "[");
            }
            Int arrayLength = dimensions.back();
            dimensions.pop_back();
            Reference reference = newArray(arrayDescriptor, arrayLength);
            auto* arrayHeader = _heap->deref<ArrayHeader>(reference);
            for(U32 i = 0;i < arrayLength;i++){
                arrayHeader->slotAt(i).referenceValue = newMultiDimensionArray(descriptor, dimensions);
            }
            return reference;
        }
    }
};

}

#endif //TINYJ_VM_H
