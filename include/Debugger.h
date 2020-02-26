#ifndef __IR_DEBUGGER_H__
#define __IR_DEBUGGER_H__

#include <IRBasic.h>
#include <Throwable.h>
#include <ostream>

enum class RendererColors
{
    Red,
    Green,
    Yellow,
    Blue,
    Purple,
    White,
    HighLightRed,
    HighLightGreen,
    HighLightYellow,
    HighLightBlue,
    HighLightPurple,
    HighLightWhite,
    Disable
};
std::ostream& operator<<(std::ostream& os, RendererColors const& _e);

class IRDebugger : public Throwable
{
public:
    IRDebugger();
    ~IRDebugger();
    
    void disassembleIR(IRQueueType& queue);
    void disassembleFunction(std::string& ident);

private:
    /* 单行汇编代码着色器 */
    std::string asmLineRenderer(std::string& code);
    /* 单行 IR 中间表示着色器 */
    std::string IRLineRenderer(std::string& code);

private:
    struct __priv_renderer_colors_struct
    {
        RendererColors comment;
        RendererColors operators;
        RendererColors sections;
        RendererColors labels;
        RendererColors instruction;
        RendererColors registersAndReferences;
        RendererColors imms;
        RendererColors position;
        RendererColors functionName;
        RendererColors directionFlags;
        RendererColors dataTypes;
    } colors;
};

#endif /* __IR_DEBUGGER_H__ */
