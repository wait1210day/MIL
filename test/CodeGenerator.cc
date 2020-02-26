#include <asmjit/asmjit.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdexcept>

// Signature of the generated function.
typedef double (*Func)(double);

int main(int argc, char *argv[])
{
    asmjit::JitRuntime jit;
    asmjit::CodeHolder code;
    asmjit::FileLogger logger(stdout);

    code.init(jit.codeInfo());
    code.setLogger(&logger);
    asmjit::x86::Compiler cc(&code);

    cc.addFunc(asmjit::FuncSignatureT<double, double>());
    asmjit::x86::Xmm argXmm = cc.newXmm();
    asmjit::x86::Mem tmpMem = cc.newDoubleConst(asmjit::ConstPool::kScopeLocal, 3.1415);
    cc.setArg(0, argXmm);
    cc.addsd(argXmm, tmpMem);
    cc.ret(argXmm);
    cc.endFunc();
    cc.finalize();

    Func fn;
    asmjit::Error err = jit.add(&fn, &code);
    if (err)
        return 1;

    std::cout << "Native code generated at " << (void *)fn << std::endl;
    std::cout << "Global size: " << jit.getMappedCodeSize()
        << " bytes, Code size: " << code.codeSize() << " bytes" << std::endl;

    std::cout << fn(10.2) << std::endl;

    jit.release(fn);
    return 0;
}
