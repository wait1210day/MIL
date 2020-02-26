#include <Debugger.h>
#include <RuntimePool.h>
#include <string>
#include <iostream>
#include <sstream>

std::ostream& operator<<(std::ostream& os, RendererColors const & _e)
{
    switch (_e)
    {
    case RendererColors::Red: os << "\033[31m"; break;
    case RendererColors::Green: os << "\033[32m"; break;
    case RendererColors::Yellow: os << "\033[33m"; break;
    case RendererColors::Blue: os << "\033[34m"; break;
    case RendererColors::Purple: os << "\033[35m"; break;
    case RendererColors::White: os << "\033[37m"; break;
    case RendererColors::HighLightRed: os << "\033[31;1m"; break;
    case RendererColors::HighLightGreen: os << "\033[32;1m"; break;
    case RendererColors::HighLightYellow: os << "\033[33;1m"; break;
    case RendererColors::HighLightBlue: os << "\033[34;1m"; break;
    case RendererColors::HighLightPurple: os << "\033[35;1m"; break;
    case RendererColors::HighLightWhite: os << "\033[37;1m"; break;
    case RendererColors::Disable: os << "\033[0m"; break;
    }
    return os;
}

IRDebugger::IRDebugger() : THROWABLE_CLASS("IRDebugger")
{
    colors.comment = RendererColors::HighLightGreen;
    colors.dataTypes = RendererColors::Blue;
    colors.directionFlags =  RendererColors::Red;
    colors.functionName = RendererColors::HighLightBlue;
    colors.imms = RendererColors::Purple;
    colors.instruction = RendererColors::Green;
    colors.labels = RendererColors::Yellow;
    colors.operators = RendererColors::HighLightWhite;
    colors.position = RendererColors::HighLightBlue;
    colors.registersAndReferences = RendererColors::Yellow;
    colors.sections = RendererColors::Red;
}

IRDebugger::~IRDebugger()
{}

void IRDebugger::disassembleFunction(std::string& ident)
{
    /* 请求 VFManager 查找一个函数上下文 */
    IRFunction *func = vfm.queryFunction(ident);
    if (func == nullptr)
        throw ARTException(THROWABLE_THIS, "No such function to be disassembled");
    /* 检查函数的枚举类型 */
    if (func->type != FunctionTypes::Assemble)
        throw ARTException(THROWABLE_THIS, "Not a function that can be disassembled (not a JIT generated function)");
    /* 输出反汇编函数 */
    if (ctl.get(OptionsList::oDebuggerColor))
    {
        std::string line;
        /* 将代码按行拆分然后交由着色器渲染 */
        for (char& ch : func->ctx.disassembled)
        {
            if (ch == '\n')
            {
                std::cout << colors.position << (void *)func->ptr
                    << colors.functionName << "<" << func->identifier << ">: " << RendererColors::Disable;
                std::cout << asmLineRenderer(line) << std::endl;
                line.clear();
                continue;
            }
            line.push_back(ch);
        }
    }
    else
        std::cout << func->ctx.disassembled << std::endl;
}

/* 看到这段函数代码请立刻跳过，永远不要尝试理解我在 while 循环里都写了什么鬼东西 */
std::string IRDebugger::asmLineRenderer(std::string& code)
{
    std::ostringstream ss;
    std::string::iterator itr = code.begin();
    std::ostringstream tmpBuf;
    std::string::iterator tmpItr;
    bool isLabel = false;
    bool onlySection = false;
    /* currentParts 用于标记当前的汇编段 */
    int currentParts = 0;

    while (itr < code.end())
    {
        /* 预先扫描一遍是否有 label 字段 */
        tmpItr = itr;
        tmpBuf.str("");
        tmpBuf << colors.labels;
        isLabel = false;
        while (*itr != ' ' && *itr != '\t' && itr < code.end())
        {
            tmpBuf << *itr;
            if (*itr == ':')
            {
                tmpBuf << RendererColors::Disable;
                isLabel = true;
            }
            itr++;
        }

        /* 处理 label 字段 */
        if (isLabel)
        {
            ss << tmpBuf.str();
            continue;
        }
        else
        {
            /* 不是 label 字段，回滚迭代器继续解析 */
            itr = tmpItr;
        }

        if (*itr == '.')
        {
            /* 是一个 section 定义（以英文句点开头） */
            ss << colors.sections;
            /* 遇到空格或者制表符就停止复制，同时注意字符串边界保护 */
            while (*itr != ' ' && *itr != '\t' && itr < code.end())
                ss << *itr++;
            ss << RendererColors::Disable;
            itr--;
            onlySection = true;
        }
        else if (onlySection)
        {
            ss << *itr++;
            continue;
        }
        else if ((*itr >= 'a' && *itr <= 'z') || (*itr >= 'A' && *itr <= 'Z') || (*itr >= '0' && *itr <= '9'))
        {
            /* 遇到有效字符，首先判断当前的所处的段 */
            if (currentParts == 0)
            {
                /* 是一个指令 */
                ss << colors.instruction;
                /* 遇到空格结束复制，然后将当前段增加 */
                while (*itr != ' ' && *itr != '\t' && itr < code.end())
                    ss << *itr++;
                currentParts++;
                ss << RendererColors::Disable;
                itr--;
            }
            else if (currentParts >= 1)
            {
                /* 是指令的参数部分 */
                if ((*itr >= 'a' && *itr <= 'z') || (*itr >= 'A' && *itr <= 'Z'))
                {
                    /* 尝试将其解释为寄存器或者 label 符号 */
                    ss << colors.registersAndReferences;
                    while (((*itr >= 'a' && *itr <= 'z') || (*itr >= 'A' && *itr <= 'Z') || (*itr >= '0' && *itr <= '9'))
                        && itr < code.end())
                        ss << *itr++;
                    ss << RendererColors::Disable;
                    itr--;
                }
                else if (*itr == '0' && (*(itr+1) == 'x' || *(itr+1) == 'X'))
                {
                    /* 十六进制立即数 */
                    ss << colors.imms;
                    /* 对于十六进制立即数，只要发现非数字或字母的部分就认为整个数字结束 */
                    while (((*itr >= 'a' && *itr <= 'z') || (*itr >= 'A' && *itr <= 'Z') || (*itr >= '0' && *itr <= '9'))
                            && itr < code.end())
                        ss << *itr++;
                    ss << RendererColors::Disable;
                    itr--;
                }
                else if (*itr >= '0' && *itr <= '9')
                {
                    /* 十进制立即数 */
                    ss << colors.imms;
                    /* 对于十进制立即数，遇到非数字字符就认为整个数字结束 */
                    while ((*itr >= '0' && *itr <= '9') && itr < code.end())
                        ss << *itr++;
                    ss << RendererColors::Disable;
                    itr--;
                }
                else
                    ss << *itr;
            }
        }
        else if (*itr == '[' || *itr == ']' || *itr == '+' || *itr == '-')
        {
            /* 特殊符号着色 */
            ss << colors.operators << *itr << RendererColors::Disable;
        }
        else if (*itr == ';' || (*itr == '/' && *(itr + 1) == '/'))
        {
            /* 注释着色。由「,」和「//」标记的单行注释将使整行无效 */
            ss << colors.comment;
            while (itr < code.end())
                ss << *itr++;
            ss << RendererColors::Disable;
        }
        else
        {
            ss << *itr;
        }

        itr++;
    }
    return ss.str();
}

/*
std::string IRLineRenderer(std::string& code)
{
    std::string::iterator itr = code.begin();

}
*/