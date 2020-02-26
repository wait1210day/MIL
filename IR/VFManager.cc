#include <VFManager.h>
#include <iostream>

DEF_INTERNAL_FUNC(IRNullptrProtector)
{
    std::cerr << "IRNullptrProtector: Null Pointer Dereference" << std::endl;
    return 0;
}

VFManager::VFManager() : THROWABLE_CLASS("VFManager")
{
    // std::cout << std::endl;
}

VFManager::~VFManager()
{
    /* 释放所有的函数、变量 */
    for (IRVariable *& ptr : vars)
    {
        noticeVariableDestroyed(ptr->identifier);
        delete ptr;
        ptr = nullptr;
    }

    for (IRFunction *& ptr : funcs)
    {
        noticeFunctionDestroyed(ptr->identifier);
        tryReleaseASMFunction(ptr);
        delete ptr->lvar;
        delete ptr;
        ptr = nullptr;
    }
}

IRFunction *VFManager::newIRFunction(std::string &ident)
{
    IRFunction *ret = new IRFunction;
    ret->identifier = ident;
    ret->ptr = (FunctionPtr) IRNullptrProtector;
    return ret;
}

IRVariable *VFManager::newIRVariable(std::string &ident, IRVariableScope scope)
{
    IRVariable *ret = new IRVariable;
    ret->identifier = ident;
    ret->interrupter = nullptr;
    ret->scope = scope;
    ret->value = 0;
    return ret;
}

IRFunction *VFManager::queryFunction(std::string &ident)
{
    for (int i = 0; i < funcs.size(); i++)
        if (funcs[i]->identifier == ident)
            return funcs[i];
    return nullptr;
}

IRVariable *VFManager::queryVariable(std::string &ident)
{
    for (int i = 0; i < vars.size(); i++)
        if (vars[i]->identifier == ident)
            return vars[i];
    return nullptr;
}

void VFManager::deleteFunction(std::string &ident)
{
    std::vector<IRFunction*>::iterator itr;
    for (itr = funcs.begin(); itr != funcs.end(); itr++)
    {
        if ((*itr)->identifier == ident)
        {
            noticeFunctionDestroyed((*itr)->identifier);
            tryReleaseASMFunction(*itr);
            /* 同时销毁其局部变量表 */
            delete (*itr)->lvar;
            delete *itr;
            itr = funcs.erase(itr);
        }
        if (itr == funcs.end())
            break;
    }
}

void VFManager::deleteVariable(std::string &ident)
{
    std::vector<IRVariable*>::iterator itr;
    for (itr = vars.begin(); itr != vars.end(); itr++)
    {
        if ((*itr)->identifier == ident)
        {
            noticeVariableDestroyed((*itr)->identifier);
            delete *itr;
            itr = vars.erase(itr);
        }
        if (itr == vars.end())
            break;
    }
}

void VFManager::acceptIRFunction(IRFunction *ptr)
{
    IRFunction *rf = queryFunction(ptr->identifier);
    if (rf)
    {
        deleteFunction(rf->identifier);
        std::cout << "\033[32;1m[-]\033[0m \033[1mVFManager:\033[0m Function \'"
            << ptr->identifier << "\' will be redefined" << std::endl;
    }
    funcs.push_back(ptr);
}

void VFManager::defineFunction(std::string &ident, FunctionPtr handler)
{
    IRFunction *irf = new IRFunction;
    irf->type = FunctionTypes::Internal;
    irf->identifier = ident;
    irf->ptr = handler;
    funcs.push_back(irf);
}

void VFManager::defineVariable(std::string &ident, double value, SystemFunction interp, IRVariableScope scope)
{
    IRVariable *iv = queryVariable(ident);
    if (queryVariable(ident))
    {
        if (iv->scope != scope)
            throw ARTException(THROWABLE_THIS, "Try changing the scope of variable while redefining.");
        iv->interrupter = interp;
        iv->value = value;
    }
    else
    {
        IRVariable *irv = new IRVariable;
        irv->identifier = ident;
        irv->interrupter = interp;
        irv->scope = scope;
        irv->value = value;
        vars.push_back(irv);
    }
}

void VFManager::tryReleaseASMFunction(IRFunction *func)
{
    if (func->type == FunctionTypes::Assemble)
    {
        if (func->ptr != nullptr && func->ctx.rt != nullptr)
            func->ctx.rt->release(func->ptr);
        if (func->ctx.cc != nullptr)
            delete func->ctx.cc;
        if (func->ctx.code != nullptr)
            delete func->ctx.code;
        if (func->ctx.rt != nullptr)
            delete func->ctx.rt;
    }
}

void VFManager::noticeVariableDestroyed(std::string& ident)
{
    std::cout << "\033[32;1m[-]\033[0m \033[1mVFManager:\033[0m Variable \'"
            << ident << "\' will be destroyed" << std::endl;
}

void VFManager::noticeFunctionDestroyed(std::string& ident)
{
    std::cout << "\033[32;1m[-]\033[0m \033[1mVFManager:\033[0m Function \'"
            << ident << "\' will be destroyed" << std::endl;
}

std::vector<IRFunction *>& VFManager::getFunctions()
{ return this->funcs; }

std::vector<IRVariable *>& VFManager::getVariables()
{ return this->vars; }
