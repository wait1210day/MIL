#ifndef __VFMANAGER__
#define __VFMANAGER__

#include <string>
#include <vector>
#include <IRBasic.h>
#include <Throwable.h>

class VFManager : public Throwable
{
public:
    VFManager();
    ~VFManager();

    IRFunction *newIRFunction(std::string &ident);
    IRFunction *queryFunction(std::string &ident);
    void deleteFunction(std::string &ident);
    void acceptIRFunction(IRFunction *ptr);
    void defineFunction(std::string &ident, FunctionPtr handler);

    IRVariable *newIRVariable(std::string &ident, IRVariableScope scope = IRVariableScope::GLOBAL);
    IRVariable *queryVariable(std::string &ident);
    void deleteVariable(std::string &ident);
    void defineVariable(std::string &ident, double value, SystemFunction interp, IRVariableScope scope);

    std::vector<IRFunction *>& getFunctions();
    std::vector<IRVariable *>& getVariables();

private:
    void tryReleaseASMFunction(IRFunction *func);
    void noticeVariableDestroyed(std::string& ident);
    void noticeFunctionDestroyed(std::string& ident);

private:
    std::vector<IRFunction *> funcs;
    std::vector<IRVariable *> vars;
};

#endif /* __VFMANAGER__ */
