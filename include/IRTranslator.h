#ifndef __IR_TRANSLATOR_H__
#define __IR_TRANSLATOR_H__

#include <asmjit/asmjit.h>
#include <IRBasic.h>
#include <Throwable.h>
#include <vector>

#define IRT_STACK_SIZE  256

/**
 * 将 MIRL 语言（详见 docs/Matbar IR Language.txt）翻译成
 * x86_64 机器语言。使用 disassembler 方法可以获得机器语言反汇编
 * 后的结果。
*/
class IRTranslator : public Throwable
{
public:
    IRTranslator(IRQueueType *irq, IRFunction *function);
    ~IRTranslator();

    /**
     * @brief: 设置参数，准备开始翻译。
     * @param stackSize: 控制栈的大小，默认为 256 bytes。
     * @return: void
    */
    void prepare(uint32_t stackSize = IRT_STACK_SIZE);
    /**
     * @brief: 完成最后的收尾工作。包括设置返回寄存器、恢复调用者函数栈帧、
     * 插入异常处理函数等等。
     * @param none.
     * @return: void.
    */
    void finalize();
    /**
     * @brief: 将指令片段逐句翻译。
     * @param none.
     * @return: void.
    */
    void translate();
    /**
     * @brief: 仅翻译一条语句。
     * @param ir: 需要被翻译的指令。
     * @param st: 当前的运算栈。
     * @return: void.
    */
    void statement(IRInstruction& ir, asmjit::x86::Mem& st);

private:
    /* 对虚拟栈进行操作（vs = virtual stack） */
    void vsPop();
    void vsTop(asmjit::x86::Xmm& reg, asmjit::x86::Mem& st);
    void vsPush(asmjit::x86::Xmm& reg, asmjit::x86::Mem& st);
    /* 检查虚拟栈是否为空，空栈将引发异常 */
    void vsEmptyThrow(asmjit::x86::Mem& st);
    /* 调用一个特定地址的函数，其原型必须是 double func(double) 或 double func(double, double) */
    void asmCallAddress(uint64_t dst, asmjit::x86::Xmm& farg, asmjit::x86::Xmm& fret);
    void asmCallAddress(uint64_t dst, asmjit::x86::Xmm& farg0, asmjit::x86::Xmm& farg1, asmjit::x86::Xmm& fret);
    /* 抛出异常（该函数是汇编与 C++ 的接口） */
    void asmThrowExcecption(int err);

private:
    /* IRQ: Intermediate Presentation Queue */
    IRQueueType *irq;
    /* 翻译完成且可执行的函数所在的内存页指针 */
    IRFunction                 *function;
    /**
     * AssemblyFunction 描述一个被翻译过来的函数，包含了用于生成函数
     * 的相关数据结构。例如 JitRuntime, Compiler 以及 CodeHolder。
    */
    AssemblyFunction           *context;
    asmjit::StringLogger       logger;
    bool                       successful;
    asmjit::x86::Xmm           arg;
    asmjit::x86::Xmm           ret;
    asmjit::x86::Gp            top;
    asmjit::x86::Gp            errReg;
    asmjit::x86::Mem           stack;
    asmjit::Label              errLabel;
    asmjit::x86::Mem           negConst;

    // asmjit::x86::Mem constant;
};

#endif /* __IR_TRANSLATOR_H__ */
