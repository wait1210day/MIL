#ifndef __IR_GENERATOR__
#define __IR_GENERATOR__

#include <IRBasic.h>
#include <Throwable.h>
#include <vector>

/* IRGenerator 模块定义了生成 IR 中间表示代码的一些必须函数 */
class IRGenerator : public Throwable
{
public:
    IRGenerator();
    void neg();
    void add();
    void sub();
    void mul();
    void div();
    void pow();
    void drop();
    void call(char *& val);
    void def(char *& name, char *& lvar);
    void pop(char *& val);
    void pushInt(char *& val);
    void pushFloat(char *& val);
    void pushVariable(char *& val);
    void final();
    void instString();

private:
    bool isThereLocalVariable();
    void freeOriginParam();

private:
    IRQueueType IRQueue;
    IRInstruction finalInst;
    std::string finalStr;
};

#endif /* __IR_GENERATOR__ */
