#ifndef __IR_OPTIMIZER_H__
#define __IR_OPTIMIZER_H__

#include <IRBasic.h>
#include <Throwable.h>
#include <vector>

typedef IRQueueType::iterator IRInstructionIterator;

/**
 * IROptimizer: 中间代码优化器。
*/
class IROptimizer : public Throwable
{
public:
    IROptimizer(IRQueueType *irq);
    ~IROptimizer();

    void checkReferences(bool isDef = false, IRFunction *thisFunc = nullptr);
    void variablesDereference();
    void prexecution();

private:
    void defCheckReferences(IRFunction *func);
    void normalCheckReferences();
    void __prexecution();
    IRInstructionIterator __do_prexecution(std::vector<IRInstructionIterator> &vst,
                          IRInstructionIterator current);
    IRInstructionIterator __replace_codeblock(IRInstructionIterator start,
                             IRInstructionIterator end,
                             IRInstruction &replace);
    IRInstruction __try_calculate(std::vector<IRInstructionIterator> &vec,
                                   IROperationCode opcode);
    bool __is_args_optimizable(std::vector<IRInstructionIterator> &vec);

private:
    IRQueueType *irq;
    bool optimized;
};

#endif /* __IR_OPTIMIZER_H__ */
