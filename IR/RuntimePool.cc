#include <RuntimePool.h>

IRGenerator codeGen;
VFManager vfm;
Controller ctl;
int IRTRuntimeError;

void cleanErrorFlag()
{ IRTRuntimeError = 0; }

std::ostream& operator <<(std::ostream& os, IRTRuntimeErrors& _e)
{
    switch (_e)
    {
    case IRTRuntimeErrors::EmptyStack:
        os << "Stack is empty";
        break;
    case IRTRuntimeErrors::StackOverflow:
        os << "Stack overflow";
        break;
    }
    return os;
}
