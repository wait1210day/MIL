#ifndef __FUNCTION_CONTEXT_H__
#define __FUNCTION_CONTEXT_H__

#include <asmjit/core/jitruntime.h>
#include <asmjit/core/codeholder.h>
#include <asmjit/x86/x86compiler.h>
#include <string>
#include <vector>
#include <ostream>

enum class FunctionTypes
{
    Internal, Assemble
};

class AssemblyFunction
{
public:
    AssemblyFunction();
    ~AssemblyFunction();
    bool available;
    asmjit::JitRuntime    *rt;
    asmjit::CodeHolder    *code;
    asmjit::x86::Compiler *cc;
    std::string           disassembled;
    std::vector<uint64_t> globalReferenced;

    friend std::ostream& operator<<(std::ostream& os, AssemblyFunction const& asmfunc);
};

std::ostream& operator<<(std::ostream& os, FunctionTypes const& _e);

#endif /* __FUNCTION_CONTEXT_H__ */
