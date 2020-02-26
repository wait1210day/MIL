#include <IRGenerator.h>
#include <RuntimePool.h>
#include <IRExecution.h>
#include <IRTranslator.h>
#include <IROptimizer.h>
#include <iostream>

#define DEF_FUNC_NOARG(rettype, name, op) \
    rettype name() \
    { \
        IRInstruction ir; \
        ir.opcode = op; \
        ir.paramType = IRTypes::NONE; \
        finalInst = ir; \
        IRQueue.push_back(ir); \
    }

IRGenerator::IRGenerator() : THROWABLE_CLASS("IRGenerator")
{
    finalInst.opcode = IROperationCode::OPCODE_BAD;
}

DEF_FUNC_NOARG(void, IRGenerator::add, IROperationCode::OPCODE_ADD);
DEF_FUNC_NOARG(void, IRGenerator::neg, IROperationCode::OPCODE_NEG);
DEF_FUNC_NOARG(void, IRGenerator::sub, IROperationCode::OPCODE_SUB);
DEF_FUNC_NOARG(void, IRGenerator::mul, IROperationCode::OPCODE_MUL);
DEF_FUNC_NOARG(void, IRGenerator::div, IROperationCode::OPCODE_DIV);
DEF_FUNC_NOARG(void, IRGenerator::pow, IROperationCode::OPCODE_POW);

void IRGenerator::drop()
{
    finalInst.opcode = IROperationCode::OPCODE_DROP;
    finalInst.paramType = IRTypes::NONE;
}

void IRGenerator::call(char *& val)
{
    IRInstruction ir;
    ir.opcode = IROperationCode::OPCODE_CALL;
    ir.paramType = IRTypes::FUNCTION;
    ir.param.func = nullptr;
    ir.tempIdentifier = val;
    free(val);
    IRQueue.push_back(ir);
}

void IRGenerator::def(char *& name, char *& lvar)
{
    finalInst.opcode = IROperationCode::OPCODE_DEF;
    finalInst.paramType = IRTypes::FUNCTION;
    finalInst.param.func = nullptr;

    finalInst.tempIdentifier = name;
    finalStr = (char *)(lvar + 1);
    free(name);
    free(lvar);
}

void IRGenerator::pop(char *& val)
{
    finalInst.opcode = IROperationCode::OPCODE_POP;
    finalInst.paramType = IRTypes::VARIABLE;
    finalInst.param.var = nullptr;
    finalInst.tempIdentifier = val;
    free(val);
}

void IRGenerator::pushInt(char *& val)
{
    IRInstruction ir;
    ir.opcode = IROperationCode::OPCODE_PUSH;
    ir.paramType = IRTypes::CONSTANT;
    ir.param.constant = atof(val);
    IRQueue.push_back(ir);
    free(val);
}

void IRGenerator::pushFloat(char *& val)
{
    IRInstruction ir;
    ir.opcode = IROperationCode::OPCODE_PUSH;
    ir.paramType = IRTypes::CONSTANT;
    ir.param.constant = atof(val);
    IRQueue.push_back(ir);
    free(val);
}

void IRGenerator::pushVariable(char *& val)
{
    IRInstruction ir;
    ir.opcode = IROperationCode::OPCODE_PUSH;
    ir.paramType = IRTypes::VARIABLE;
    ir.param.var = nullptr;
    ir.tempIdentifier = val;
    IRQueue.push_back(ir);
    free(val);
}

void IRGenerator::final()
{
    /* 先输出一遍 IR 的反编译结果 */
    // instString();

    /* 建立一个代码优化器对象 */
    IROptimizer optimizer(&IRQueue);
    IRFunction *definedFunction = nullptr;
    /* 建立一个 IR 虚拟机执行上下文 */
    IRExecution execute;
    /* 汇编翻译器对象指针 */
    IRTranslator *translate = nullptr;

    try
    {
        switch (finalInst.opcode)
        {
        case IROperationCode::OPCODE_DROP:
            optimizer.checkReferences();
            optimizer.variablesDereference();
            /* 建立执行上下文，执行 IR 代码 */
            execute.load(IRQueue);
            execute.execution();
            /* 输出结果 */
            std::cout << (double)execute.getOutput() << std::endl;
            break;

        case IROperationCode::OPCODE_POP:
            optimizer.checkReferences();
            optimizer.variablesDereference();

            /* 建立执行上下文，执行 IR 代码 */
            execute.load(IRQueue);
            execute.execution();
            /* 保存结果到全局变量表 */
            vfm.defineVariable(finalInst.tempIdentifier, execute.getOutput(), nullptr, IRVariableScope::GLOBAL);
            break;

        case IROperationCode::OPCODE_DEF:
            /* 首先建立函数上下文以及它的的本地变量表 */
            definedFunction = vfm.newIRFunction(finalInst.tempIdentifier);
            definedFunction->lvar = vfm.newIRVariable(finalStr, IRVariableScope::LOCAL);
            /* 检查引用的所有全局变量和函数 */
            optimizer.checkReferences(true, definedFunction);
            optimizer.prexecution();

            /* 开始翻译 */
            translate = new IRTranslator(&IRQueue, definedFunction);
            translate->prepare();
            translate->translate();
            translate->finalize();
            /* 注册函数 */
            vfm.acceptIRFunction(definedFunction);
            definedFunction = nullptr;
            break;

        default:
            throw ARTException(THROWABLE_THIS, "Unexpected instruction");
            break;
        }
    }
    catch (ARTException& e)
    {
        if (translate) delete translate;
        if (definedFunction)
        {
            if (definedFunction->lvar) delete definedFunction->lvar;
            delete definedFunction;
        }
        IRQueue.clear();
        finalInst.tempIdentifier.clear();
        finalStr.clear();
        std::rethrow_exception(std::current_exception());
    }

    if (translate) delete translate;
    if (definedFunction)
    {
        if (definedFunction->lvar) delete definedFunction->lvar;
            delete definedFunction;
    }
    IRQueue.clear();
    finalInst.tempIdentifier.clear();
    finalStr.clear();
}

void IRGenerator::instString()
{
    IRQueue.push_back(finalInst);
    for (IRInstruction& ir : IRQueue)
    {
        std::cout << ir.opcode;
        if (ir.paramType != IRTypes::NONE)
        {
            std::cout << "[" << ir.paramType;
            if (ir.paramType == IRTypes::CONSTANT)
            {
                std::cout << "] " << ir.param.constant;
            }
            else if (ir.paramType == IRTypes::FUNCTION)
            {
                if (ir.opcode == IROperationCode::OPCODE_DEF)
                    std::cout << "] " << finalInst.tempIdentifier << "[" << finalStr << "]";
                else
                    std::cout << "] " << ir.tempIdentifier;
            }
            else if (ir.paramType == IRTypes::VARIABLE)
            {
                if (ir.opcode == IROperationCode::OPCODE_POP)
                    std::cout << "] " << finalInst.tempIdentifier;
                else
                    std::cout << "] " << ir.tempIdentifier;
            }
        }
        std::cout << std::endl;
    }
    IRQueue.pop_back();
}
