#include <IROptimizer.h>
#include <RuntimePool.h>
#include <Throwable.h>
#include <iostream>
#include <sstream>
#include <cmath>

#define TRY_BREAK \
    if (optimized) break;

IROptimizer::IROptimizer(IRQueueType *irq)
    : THROWABLE_CLASS("IROptimizer")
{
    this->irq = irq;
}

IROptimizer::~IROptimizer()
{
}

void IROptimizer::variablesDereference()
{
    for (IRInstruction &ir : *irq)
    {
        if (ir.paramType == IRTypes::VARIABLE)
        {
            ir.param.var = vfm.queryVariable(ir.tempIdentifier);

            if (ir.param.var == nullptr)
            {
                std::ostringstream ss;
                ss << "Undefined variable \'" << ir.tempIdentifier << "\'. ";
                ss << "Using \'name=expression\' to define";
                throw ARTException(*this, ss.str());
            }

            double val = ir.param.var->value;
            ir.paramType = IRTypes::CONSTANT;
            ir.param.constant = val;
        }
    }
}

void IROptimizer::checkReferences(bool isDef, IRFunction *thisFunc)
{
    if (isDef)
        defCheckReferences(thisFunc);
    else
        normalCheckReferences();
}

void IROptimizer::defCheckReferences(IRFunction *func)
{
    bool localReferenced = false;
    for (IRInstruction &ir : *irq)
    {
        if (ir.paramType == IRTypes::VARIABLE)
        {
            if (func->lvar->identifier == ir.tempIdentifier)
            {
                ir.param.var = func->lvar;
                localReferenced = true;
            }
            else
            {
                ir.param.var = vfm.queryVariable(ir.tempIdentifier);
                if (ir.param.var == nullptr)
                {
                    std::ostringstream ss;
                    ss << "Undefined variable \'" << ir.tempIdentifier << "\'. ";
                    ss << "Using \'name=expression\' to define";
                    throw ARTException(*this, ss.str());
                }
            }
        }
        else if (ir.paramType == IRTypes::FUNCTION)
        {
            ir.param.func = vfm.queryFunction(ir.tempIdentifier);
            if (ir.param.func == nullptr)
            {
                std::ostringstream ss;
                ss << "Undefined function \'" << ir.tempIdentifier << "\'. ";
                ss << "Using \'name[x]=expression\' to define";
                throw ARTException(*this, ss.str());
            }
        }
    }
    if (!localReferenced)
        throw ARTException(THROWABLE_THIS, "Function does not reference any local arguments");
}

void IROptimizer::normalCheckReferences()
{
    for (IRInstruction &ir : *irq)
    {
        if (ir.paramType == IRTypes::VARIABLE)
        {
            ir.param.var = vfm.queryVariable(ir.tempIdentifier);
            if (ir.param.var == nullptr)
            {
                std::ostringstream ss;
                ss << "Undefined variable \'" << ir.tempIdentifier << "\'. ";
                ss << "Using \'name=expression\' to define";
                throw ARTException(*this, ss.str());
            }
        }
        else if (ir.paramType == IRTypes::FUNCTION)
        {
            ir.param.func = vfm.queryFunction(ir.tempIdentifier);
            if (ir.param.func == nullptr)
            {
                std::ostringstream ss;
                ss << "Undefined function \'" << ir.tempIdentifier << "\'. ";
                ss << "Using \'name[x]=expression\' to define";
                throw ARTException(*this, ss.str());
            }
        }
    }
}

void IROptimizer::prexecution()
{
    __prexecution();
}

void IROptimizer::__prexecution()
{
    std::vector<IRInstructionIterator> vst;
    IRInstructionIterator begin = irq->begin();
    /* rep means replace */
    IRInstructionIterator repBegin;
    IRInstruction inst;
    optimized = true;

    while (optimized)
    {
        vst.clear();
        begin = irq->begin();
        optimized = false;

        while (begin != irq->end() && !optimized)
        {
            switch (begin->opcode)
            {
            case IROperationCode::OPCODE_ADD:
                begin = __do_prexecution(vst, begin);
                break;
            case IROperationCode::OPCODE_SUB:
                begin = __do_prexecution(vst, begin);
                break;
            case IROperationCode::OPCODE_MUL:
                begin = __do_prexecution(vst, begin);
                break;
            case IROperationCode::OPCODE_DIV:
                begin = __do_prexecution(vst, begin);
                break;
            case IROperationCode::OPCODE_POW:
                begin = __do_prexecution(vst, begin);
                break;
            case IROperationCode::OPCODE_NEG:
                if (vst.back()->paramType != IRTypes::CONSTANT)
                {
                    /* 代码块不可优化 */
                    vst.pop_back();
                    optimized  = false;
                    break;
                }
                inst = *vst.back();
                inst.param.constant = -inst.param.constant;
                repBegin = *vst.begin();

                begin = __replace_codeblock(repBegin, begin, inst);
                optimized = true;

                break;
            case IROperationCode::OPCODE_PUSH:
                vst.push_back(begin);
                break;
            default:
                break;
            }
            begin++;
        }
    }
    vst.clear();
}

IRInstructionIterator IROptimizer::__do_prexecution(std::vector<IRInstructionIterator> &vst,
                                   IRInstructionIterator current)
{
    IRInstruction inst;
    IRInstructionIterator repBegin;

    if (!__is_args_optimizable(vst))
    {
        /* 代码块不可优化 */
        if (vst.size() >= 2)
        {
            vst.pop_back();
            vst.pop_back();
        }
        else if (vst.size() == 1)
        {
            vst.pop_back();
        }
        optimized = false;
        return current;
    }
    inst = __try_calculate(vst, current->opcode);
    repBegin = *(vst.end()-2);

    optimized = true;
    return __replace_codeblock(repBegin, current, inst);
}

IRInstructionIterator IROptimizer::__replace_codeblock(IRInstructionIterator start,
                                      IRInstructionIterator end,
                                      IRInstruction &replace)
{
    end++;
    start = irq->erase(start, end);
    // start = irq->erase(end);
    // start--;
    return irq->insert(start, replace);
    // irq->erase(start);
}

IRInstruction IROptimizer::__try_calculate(std::vector<IRInstructionIterator> &vec,
                                           IROperationCode opcode)
{
    double o1, o2;
    IRInstruction inst;
    IRInstructionIterator vo1, vo2;
    vo2 = vec[vec.size() - 1];
    vo1 = vec[vec.size() - 2];

    o1 = vo1->param.constant;
    o2 = vo2->param.constant;
    inst.opcode = IROperationCode::OPCODE_PUSH;
    inst.paramType = IRTypes::CONSTANT;
    
    switch (opcode)
    {
    case IROperationCode::OPCODE_ADD:
        inst.param.constant = o1 + o2;
        break;
    case IROperationCode::OPCODE_SUB:
        inst.param.constant = o1 - o2;
        break;
    case IROperationCode::OPCODE_MUL:
        inst.param.constant = o1 * o2;
        break;
    case IROperationCode::OPCODE_DIV:
        inst.param.constant = o1 / o2;
        break;
    case IROperationCode::OPCODE_POW:
        inst.param.constant = pow(o1, o2);
        break;
    default:
        break;
    }

    return inst;
}

bool IROptimizer::__is_args_optimizable(std::vector<IRInstructionIterator> &vec)
{
    /**
     * 定义一个指令片段 I，其值依赖于另一个指令片段 V，
     * 如果指令片段 V 是不可以安全优化的，那么指令片段 I 也一定是不可安全优化的。
     * 在这里表现为 vec 迭代器向量长度不足 2.
    */
    if (vec.size() < 2)
        return false;

    IRInstructionIterator vo1, vo2;
    vo2 = vec[vec.size() - 1];
    vo1 = vec[vec.size() - 2];

    return (vo2->paramType == IRTypes::CONSTANT && vo1->paramType == IRTypes::CONSTANT);
}
