#include <IRBasic.h>

std::ostream& operator <<(std::ostream& os, IRTypes const & _e)
{
    switch (_e)
    {
    case IRTypes::CONSTANT:
        os << "CONSTANT";
        break;
    case IRTypes::FUNCTION:
        os << "FUNCTION";
        break;
    case IRTypes::NONE:
        os << "NONE";
        break;
    case IRTypes::VARIABLE:
        os << "VARIABLE";
        break;
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, IRVariableScope const & _e)
{
    switch (_e)
    {
    case IRVariableScope::GLOBAL:
        os << "GLOBAL";
        break;
    case IRVariableScope::LOCAL:
        os << "LOCAL";
        break;
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, IROperationCode const & _e)
{
    switch (_e)
    {
        case IROperationCode::OPCODE_ADD: os << "ADD"; break;
        case IROperationCode::OPCODE_BAD: os << "BAD"; break;
        case IROperationCode::OPCODE_CALL: os << "CALL"; break;
        case IROperationCode::OPCODE_DEF: os << "DEF"; break;
        case IROperationCode::OPCODE_DIV: os << "DIV"; break;
        case IROperationCode::OPCODE_DROP: os << "DROP"; break;
        case IROperationCode::OPCODE_MUL: os << "MUL"; break;
        case IROperationCode::OPCODE_NEG: os << "NEG"; break;
        case IROperationCode::OPCODE_POP: os << "POP"; break;
        case IROperationCode::OPCODE_POW: os << "POW"; break;
        case IROperationCode::OPCODE_PUSH: os << "PUSH"; break;
        case IROperationCode::OPCODE_SUB: os << "SUB"; break;
    }
    return os;
}
