#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <Throwable.h>

enum class OptionsList
{
    oExceptionTerminate,
    oExceptionDivideByZero,
    oDebuggerIRDump,
    oDebuggerColor,
    oLexicalGuessThreshold,
    oNone
};

struct OptionsMap
{
    OptionsList opt;
    bool b_val;
    double o_val;
};

enum class ControllerIdentifier
{
    kTerminate,
    kGetMIRLStandard,
    kGetMILStandard,
    kGetHelp,
    kGetAuthor,
    kDebuggerDisassemble,
    kDebuggerIRDump,
    kDebuggerColor,
    kEnumerationCommands,
    kEnumerationVariables,
    kEnumerationFunctions,
    kExceptionTerminate,
    kExceptionIgnore,
    kExceptionDivideByZero,
    kLexicalGuessThresold,
    kEcho
};

struct ControllerTargetsMap
{
    ControllerIdentifier identifier;
    bool expectedArgument;
    char const *cmd;
    char const *usage;
    char const *help;
    OptionsList option;
};

class Controller : public Throwable
{
public:
    Controller();
    ~Controller();

    void parse(char const *cmd);
    void parse(char const *cmd, char const *arg0);

    // void selector(ControllerTargetsMap& map, char const *arg0);

    bool get(OptionsList ol);
    double oget(OptionsList ol);
    void set(OptionsList ol, bool value);
    void set(OptionsList ol, double value);

private:
    void functionsEnumerator();
    void variablesEnumerator();
    void commandsEnumerator();
    void _parse(char const *cmd, char const *arg0);
    void out(std::string& _str);
    void out(std::string&& _str);
    void out(char const *_str);
    OptionsMap& findOption(OptionsList ol);
    bool getBoolean(std::string& str);
    double getNumber(std::string& str);
    std::string getString(std::string& str);
    char const *tryGivingSuggestion(std::string& cmd);
    
    template<typename _T>
    _T **allocTable(size_t x, size_t y);
    template<typename _T>
    void deleteTable(_T **ptr, size_t x, size_t y);

private:
    void help(std::string& arg);
};

#endif /* __CONTROLLER_H__ */
