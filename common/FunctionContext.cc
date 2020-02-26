#include <FunctionContext.h>

std::ostream& operator<<(std::ostream& os, AssemblyFunction const& asmfunc)
{
    if (!asmfunc.available)
    {
        os << "unavailable";
        return os;
    }

    os << "Architecture " << asmfunc.rt->codeInfo().archId() << ", ";
    os << asmfunc.globalReferenced.size() << " global references";
    return os;
}

std::ostream& operator<<(std::ostream& os, FunctionTypes const& _e)
{
    switch (_e)
    {
    case FunctionTypes::Assemble: os << "Assembly"; break;
    case FunctionTypes::Internal: os << "Internal"; break;
    }
    return os;
}

AssemblyFunction::AssemblyFunction()
{
    available = false;
    rt = nullptr;
    code = nullptr;
    cc = nullptr;
}

AssemblyFunction::~AssemblyFunction()
{
}
