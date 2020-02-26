#ifndef __IR_BASIC_H__
#define __IR_BASIC_H__

#include <string>
#include <FunctionContext.h>
#include <ostream>

#define DEF_INTERNAL_FUNC(ident) double ident(double arg)
#define DEF_SYSTEM_FUNC(ident)   void ident(double arg)

typedef double (*FunctionPtr)(double arg);
typedef void (*SystemFunction)(double newValue);

enum class IRTypes
{
    VARIABLE,
    CONSTANT,
    FUNCTION,
    NONE
};

enum class IRVariableScope
{
    GLOBAL,
    LOCAL
};

struct IRVariable
{
    std::string identifier;
    SystemFunction interrupter;
    IRVariableScope scope;
    double value;
};

struct IRFunction
{
    std::string identifier;
    FunctionTypes type;
    FunctionPtr ptr;
    AssemblyFunction ctx;
    IRVariable *lvar;
};

enum class IROperationCode
{
    OPCODE_ADD  = 1,
    OPCODE_SUB  = 2,
    OPCODE_PUSH = 3,
    OPCODE_POP  = 4,
    OPCODE_DEF  = 5,
    OPCODE_CALL = 6,
    OPCODE_NEG  = 7,
    OPCODE_MUL  = 8,
    OPCODE_DIV  = 9,
    OPCODE_POW  = 10,
    OPCODE_DROP = 11,
    OPCODE_BAD  = 12
};

union IRInstArgument
{
    double constant;
    IRVariable *var;
    IRFunction *func;
};

struct IRInstruction
{
    /* 指令的基本操作码 */
    IROperationCode opcode;
    /* 指令的唯一参数 */
    IRInstArgument param;
    /* 唯一参数的类型 */
    IRTypes paramType;
    std::string tempIdentifier;
};

typedef std::vector<IRInstruction> IRQueueType;

std::ostream& operator <<(std::ostream& os, IRTypes const & _e);
std::ostream& operator <<(std::ostream& os, IRVariableScope const & _e);
std::ostream& operator <<(std::ostream& os, IROperationCode const & _e);

#endif /* __IR_BASIC_H__ */
