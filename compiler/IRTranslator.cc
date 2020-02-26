#include <RuntimePool.h>
#include <IRTranslator.h>
#include <iostream>
#include <cmath>
#include <sstream>

void asmSetErrorFlag(int err)
{
    /* 设置异常标识 */
    IRTRuntimeError = -err;
}

IRTranslator::IRTranslator(IRQueueType *irq, IRFunction *function)
    : THROWABLE_CLASS("IRTranslator")
{
    this->irq = irq;
    this->function = function;
    this->successful = false;
    this->function->type = FunctionTypes::Assemble;
    context = &this->function->ctx;
    context->rt = new asmjit::JitRuntime;
    context->code = new asmjit::CodeHolder;
    context->code->init(context->rt->codeInfo());
    context->code->setLogger(&logger);
    context->cc = new asmjit::x86::Compiler(context->code);

    logger.setFlags(asmjit::FormatOptions::kFlagHexImms
        | asmjit::FormatOptions::kFlagHexOffsets
        | asmjit::FormatOptions::kFlagExplainImms
        | asmjit::FormatOptions::kFlagAnnotations);
    logger.setIndentation(asmjit::FormatOptions::kIndentationCode, 4);
}

IRTranslator::~IRTranslator()
{
    if (!this->successful)
    {
        if (function->ptr != nullptr)
            context->rt->release(function->ptr);
        delete context->rt;
        delete context->code;
        delete context->cc;
    }
    else
    {
        context->available = true;
    }
}

void IRTranslator::prepare(uint32_t stackSize)
{
    /* 初始化函数调用约定，以及参数寄存器和返回寄存器 */
    context->cc->addFunc(asmjit::FuncSignatureT<double, double>());
    arg = context->cc->newXmm();
    ret = context->cc->newXmm();
    /* 分配异常代码专用寄存器 */
    errReg = context->cc->newInt32();
    errLabel = context->cc->newLabel();
    /* 设置参数寄存器 */
    context->cc->setArg(0, arg);
    /* 建立指定大小的栈，以及栈顶指针寄存器 top */
    top = context->cc->newIntPtr();
    stack = context->cc->newStack(stackSize, sizeof(double));
    /* 清零栈顶寄存器 top */
    context->cc->xor_(top, top);
    /* 清零异常标识寄存器 errReg */
    context->cc->xor_(errReg, errReg);
    /* 初始化常量 */
    negConst = context->cc->newDoubleConst(asmjit::ConstPool::kScopeLocal, -1);
}

void IRTranslator::finalize()
{
    context->cc->ret(ret);

    /* 异常处理使用的 label，正常情况下不会执行到此处 */
    context->cc->bind(errLabel);
    /* 调用异常处理函数，异常代码保存在 errReg 中 */
    asmjit::FuncCallNode *func = context->cc->call(
        (uint64_t)asmSetErrorFlag, asmjit::FuncSignatureT<void, int>());
    func->setArg(0, errReg);
    context->cc->ret(ret);

    context->cc->endFunc();
    context->cc->finalize();

    asmjit::Error compilerError;
    compilerError = context->rt->add(&function->ptr, context->code);
    if (compilerError)
    {
        std::ostringstream ss;
        ss << "Failed in generating native code (error=";
        ss << compilerError << ")";
        throw ARTException(THROWABLE_THIS, ss.str());
    }
    this->successful = true;
    // std::cout << logger.data() << std::endl;
    context->disassembled = "<No Disassembler>";
    context->disassembled = logger.data();
}

void IRTranslator::translate()
{
    /* 设置栈顶索引指针，初始化操作数大小为 double */
    asmjit::x86::Mem stackIdx(stack);
    stackIdx.setIndex(top);
    stackIdx.setSize(sizeof(double));

    for (IRInstruction& ir : *irq)
        statement(ir, stackIdx);
    vsTop(ret, stackIdx);
    vsPop();
}

void IRTranslator::statement(IRInstruction& ir, asmjit::x86::Mem& st)
{
    // asmjit::x86::Mem memref;
    asmjit::x86::Xmm a = context->cc->newXmm(), b = context->cc->newXmm();

    switch (ir.opcode)
    {
    case IROperationCode::OPCODE_ADD:
        vsTop(b, st);
        vsPop();
        vsTop(a, st);
        vsPop();
        context->cc->addsd(a, b);
        vsPush(a, st);
        break;
    case IROperationCode::OPCODE_BAD:
        throw ARTException(THROWABLE_THIS, "Corrupted IR instruction");
        break;
    case IROperationCode::OPCODE_CALL:
        /* 取得参数 */
        vsTop(a, st);
        vsPop();
        /* 调用前的检查 */
        if (ir.paramType != IRTypes::FUNCTION)
            throw ARTException(THROWABLE_THIS, "call: Not a valid function type");
        if (ir.param.func == nullptr || ir.param.func->ptr == nullptr)
            throw ARTException(THROWABLE_THIS, "call: Null pointer exception");
        /* 清理异常标志 */
        context->cc->xor_(errReg, errReg);
        /* 调用并将返回值压栈 */
        asmCallAddress((uint64_t)ir.param.func->ptr, a, a);
        vsPush(a, st);
        break;
    case IROperationCode::OPCODE_DIV:
        vsTop(b, st);
        vsPop();
        vsTop(a, st);
        vsPop();
        context->cc->divsd(a, b);
        vsPush(a, st);
        break;
    case IROperationCode::OPCODE_MUL:
        vsTop(b, st);
        vsPop();
        vsTop(a, st);
        vsPop();
        context->cc->mulsd(a, b);
        vsPush(a, st);
        break;
    case IROperationCode::OPCODE_NEG:
        vsTop(a, st);
        vsPop();
        context->cc->mulsd(a, negConst);
        vsPush(a, st);
        break;
    case IROperationCode::OPCODE_POW:
        vsTop(b, st);
        vsPop();
        vsTop(a, st);
        vsPop();
        asmCallAddress((uint64_t)((long double(*)(long double, long double y))pow), a, b, a);
        vsPush(a, st);
        break;
    case IROperationCode::OPCODE_PUSH:
        /* 变量解除引用 */
        if (ir.paramType == IRTypes::VARIABLE)
        {
            if (ir.param.var->scope == IRVariableScope::GLOBAL)
            {
                /* 全局变量地址引用 */
                asmjit::x86::Gp memref = context->cc->newIntPtr();
                context->cc->mov(memref, asmjit::Imm((uint64_t)&ir.param.var->value));
                // memref = asmjit::x86::qword_ptr((uint64_t)&ir.param.var->value);
                context->cc->movsd(a, asmjit::x86::qword_ptr(memref));
                vsPush(a, st);
                context->globalReferenced.push_back((uint64_t)ir.param.var);
            }
            else
            {
                /* 局部变量作为参数引用，直接压栈 */
                vsPush(arg, st);
            }
        }
        else if (ir.paramType == IRTypes::CONSTANT)
        {
            /* 常量直接压栈 */
            context->cc->movsd(a, context->cc->newDoubleConst(asmjit::ConstPool::kScopeLocal, ir.param.constant));
            vsPush(a, st);
        }
        else
            throw ARTException(THROWABLE_THIS, "Unexpected IR data type");
        break;
    case IROperationCode::OPCODE_SUB:
        vsTop(b, st);
        vsPop();
        vsTop(a, st);
        vsPop();
        context->cc->subsd(a, b);
        vsPush(a, st);
        break;
    default:
        throw ARTException(THROWABLE_THIS, "Unexpected IR instruction");
        break;
    }
}

void IRTranslator::vsPop()
{ context->cc->sub(top, sizeof(double)); }

void IRTranslator::vsTop(asmjit::x86::Xmm& reg, asmjit::x86::Mem& st)
{ context->cc->movsd(reg, st); }

void IRTranslator::vsPush(asmjit::x86::Xmm& reg, asmjit::x86::Mem& st)
{
    context->cc->add(top, sizeof(double));
    context->cc->movsd(st, reg);
}

void IRTranslator::asmThrowExcecption(int err)
{
    /* 设置异常代码寄存器并跳转到异常处理 label */
    context->cc->mov(errReg, err);
    context->cc->jmp(errLabel);
}

void IRTranslator::asmCallAddress(uint64_t dst, asmjit::x86::Xmm& farg, asmjit::x86::Xmm& fret)
{
    asmjit::FuncCallNode *f = context->cc->call(dst,
        asmjit::FuncSignatureT<double, double>());
    f->setArg(0, farg);
    f->setRet(0, fret);
}

void IRTranslator::asmCallAddress(uint64_t dst, asmjit::x86::Xmm& farg0, asmjit::x86::Xmm& farg1, asmjit::x86::Xmm& fret)
{
    asmjit::FuncCallNode *f = context->cc->call(dst,
        asmjit::FuncSignatureT<double, double, double>());
    f->setArg(0, farg0);
    f->setArg(1, farg1);
    f->setRet(0, fret);
}
