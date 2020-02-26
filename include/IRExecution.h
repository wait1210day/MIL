#ifndef __IR_EXECUTION_H__
#define __IR_EXECUTION_H__

#include <IRBasic.h>
#include <Throwable.h>
#include <vector>
#include <stack>

class IRExecution : public Throwable
{
public:
    IRExecution();
    void load(IRQueueType& irq);
    void execution();
    double getOutput();

private:
    void executeStatement(IRInstruction& ir);
    bool throwIfError();

private:
    IRQueueType irq;
    std::stack<double> ps;
};

#endif /* __IR_EXECUTION_H__ */
