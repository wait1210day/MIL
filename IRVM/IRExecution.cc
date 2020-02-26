#include <IRExecution.h>
#include <RuntimePool.h>
#include <iostream>
#include <sstream>
#include <cmath>

#define POP_STACK_2(stack, a, b) \
    a = stack.top(); \
    stack.pop(); \
    b = stack.top(); \
    stack.pop();

IRExecution::IRExecution() : THROWABLE_CLASS("IRExecution")
{}

void IRExecution::load(IRQueueType& irq)
{
    this->irq = irq;
    while (!this->ps.empty())
        this->ps.pop();
}

void IRExecution::execution()
{
    for (IRInstruction& ir : irq)
        executeStatement(ir);
}

double IRExecution::getOutput()
{
    return ps.top();
}

void IRExecution::executeStatement(IRInstruction& ir)
{
    double a, b;
    /* 根据 OPCODE 逐句执行 IR */
    switch (ir.opcode)
    {
    case IROperationCode::OPCODE_ADD:
        POP_STACK_2(ps, a, b);
        ps.push(a + b);
        break;
    case IROperationCode::OPCODE_BAD:
        throw ARTException(THROWABLE_THIS, "Bad IR Instruction");
        break;
    case IROperationCode::OPCODE_CALL:
        a = ps.top();
        ps.pop();
        if (ir.param.func == nullptr)
            throw ARTException(THROWABLE_THIS, "Trying calling a function of null");

        /* 清除函数异常标志然后调用 */
        cleanErrorFlag();
        b = ir.param.func->ptr(a);
        /* 函数发生错误 */
        throwIfError();
        ps.push(b);
        break;
    case IROperationCode::OPCODE_DIV:
        POP_STACK_2(ps, b, a);
        if (b == 0 && ctl.get(OptionsList::oExceptionDivideByZero))
            throw ARTException(THROWABLE_THIS, "Divided by zero");
        ps.push((double)((double)a / (double)b));
        break;
    case IROperationCode::OPCODE_MUL:
        POP_STACK_2(ps, a, b);
        ps.push(a * b);
        break;
    case IROperationCode::OPCODE_NEG:
        a = ps.top();
        ps.pop();
        a *= -1;
        ps.push(a);
        break;
    case IROperationCode::OPCODE_POW:
        POP_STACK_2(ps, b, a);
        ps.push(pow(a, b));
        break;
    case IROperationCode::OPCODE_PUSH:
        if (ir.paramType != IRTypes::CONSTANT)
            throw ARTException(THROWABLE_THIS, "Corrupted type for push instruction, must be a constant");
        ps.push(ir.param.constant);
        break;
    case IROperationCode::OPCODE_SUB:
        POP_STACK_2(ps, b, a);
        ps.push(a - b);
        break;
    default:
        throw ARTException(THROWABLE_THIS, "Unexpected instruction");
        break;
    }
}

bool IRExecution::throwIfError()
{
    if (IRTRuntimeError)
    {
        IRTRuntimeErrors err = (IRTRuntimeErrors)IRTRuntimeError;
        std::ostringstream ss;
        ss << "Native error: " << err << "<assembly>";
        throw ARTException(THROWABLE_THIS, ss.str());
    }
    return false;
}
