#ifndef __RUNTIME_POOL__
#define __RUNTIME_POOL__

#include <IRGenerator.h>
#include <VFManager.h>
#include <Controller.h>
#include <ostream>

#define RTP_BUFSIZ 1024

extern IRGenerator codeGen;
extern VFManager vfm;
extern Controller ctl;
extern int IRTRuntimeError;

void cleanErrorFlag();
const char *lookupErrorFlag();

/**
 * IRTranslator 编译出的函数可能会抛出的异常
 * （汇编抛出异常通过 asmThrowable 函数作为中间层实现）。
 * IRTranslator::asmThrowExcecption 是显式的异常构造器，
 * 它将生成一句跳转汇编代码。
*/
enum class IRTRuntimeErrors
{
    StackOverflow = 1,
    EmptyStack    = 2
};

std::ostream& operator <<(std::ostream& os, IRTRuntimeErrors& _e);

#endif /* __RUNTIME_POOL__ */
