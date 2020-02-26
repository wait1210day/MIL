#include <Controller.h>
#include <Parser.h>
#include <Definitions.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <Enumerator.h>
#include <RuntimePool.h>
#include <Debugger.h>

std::vector<ControllerTargetsMap> cmdsMap = {
    {
        ControllerIdentifier::kTerminate,
        false,
        "@quit",
        "@quit <no more arguments>",
        "Exit the program correctly.",
        OptionsList::oNone
    },
    {
        ControllerIdentifier::kTerminate,
        false,
        "@exit",
        "@exit <no more arguments>",
        "Exit the program correctly.",
        OptionsList::oNone
    },
    {
        ControllerIdentifier::kEcho,
        true,
        "@echo",
        "@echo <string>",
        "Print a string to standard output.",
        OptionsList::oNone
    },
    {
        ControllerIdentifier::kGetMIRLStandard,
        false,
        "@get-MIRL-standard",
        "@get-MIRL-standard <no more arguments>",
        "Display the version information of the MIRL language standard. (See \'The Intermediate Presentation Language for Matbar\')",
        OptionsList::oNone
    },
    {
        ControllerIdentifier::kGetMILStandard,
        false,
        "@get-MIL-standard",
        "@get-MIL-standard <no more arguments>",
        "Display the version information of the MIL language standard. (See \'The Interactive Language for Matbar\')",
        OptionsList::oNone
    },
    {
        ControllerIdentifier::kGetHelp,
        true,
        "@get-help",
        "@get-help <string: the command you want>",
        "Display the help information about a command.",
        OptionsList::oNone
    },
    {
        ControllerIdentifier::kGetAuthor,
        false,
        "@get-author",
        "@get-author <no more arguments>",
        "Display author information about this program.",
        OptionsList::oNone
    },
    {
        ControllerIdentifier::kExceptionTerminate,
        true,
        "@exception-terminate",
        "@exception-terminate <boolean>",
        "Enables or disables the exit function in case of exception.",
        OptionsList::oExceptionTerminate
    },
    {
        ControllerIdentifier::kExceptionIgnore,
        true,
        "@exception-ignore",
        "@exception-ignore <boolean>",
        "Enables or disables the ignore function in case of exception.",
        OptionsList::oExceptionTerminate
    },
    {
        ControllerIdentifier::kExceptionDivideByZero,
        true,
        "@exception-divide-by-zero",
        "@exception-divide-by-zero <boolean>",
        "Whether an exception occurs when dividing by zero. \
        \nThe result will be infinity(inf or #INF) when dividing by zero if you disabled this exception.",
        OptionsList::oExceptionDivideByZero
    },
    {
        ControllerIdentifier::kEnumerationVariables,
        false,
        "@enumeration-variables",
        "@enumeration-variables <no more arguments>",
        "Enumerate all of the variables and their properties.",
        OptionsList::oNone
    },
    {
        ControllerIdentifier::kEnumerationFunctions,
        false,
        "@enumeration-functions",
        "@enumeration-functions <no more arguments>",
        "Enumerate all of the functions and their properties.",
        OptionsList::oNone
    },
    {
        ControllerIdentifier::kEnumerationCommands,
        false,
        "@enumeration-commands",
        "@enumeration-commands <no more arguments>",
        "Enumerate all of the available commands and their properties.",
        OptionsList::oNone
    },
    {
        ControllerIdentifier::kDebuggerIRDump,
        true,
        "@debugger-IR-dump",
        "@debugger-IR-dump <boolean>",
        "Enables or disables the debugger's IR code real-time dump feature.",
        OptionsList::oDebuggerIRDump
    },
    {
        ControllerIdentifier::kDebuggerDisassemble,
        true,
        "@debugger-disassemble",
        "@debugger-disassemble <string: function name>",
        "Causes the debugger to immediately output the disassembly result of a function.",
        OptionsList::oNone
    },
    {
        ControllerIdentifier::kDebuggerColor,
        true,
        "@debugger-color",
        "@debugger-color <boolean>",
        "Enables or disables the output shading feature of the debugger.",
        OptionsList::oDebuggerColor
    },
    {
        ControllerIdentifier::kLexicalGuessThresold,
        true,
        "@set-lexical-guess-threshold",
        "@set-lexical-guess-threshold <double:value>",
        "Set the lexical guessing threshold for the command parser. (See \'The Interactive Language for Matbar\' \
chapter \'Command Interface: Lexical Guessing\')",
        OptionsList::oLexicalGuessThreshold
    }
};

OptionsMap optionsMap[] = {
    { OptionsList::oDebuggerColor,         true,  0  },
    { OptionsList::oDebuggerIRDump,        true,  0  },
    { OptionsList::oExceptionDivideByZero, false, 0  },
    { OptionsList::oExceptionTerminate,    false, 0  },
    { OptionsList::oLexicalGuessThreshold, false, 0.6 }
};

Controller::Controller() : THROWABLE_CLASS("Controller")
{}

Controller::~Controller()
{}

void Controller::parse(char const *cmd)
{
    _parse(cmd, nullptr);
    // free((void *)cmd);
}

void Controller::parse(char const *cmd, char const *arg0)
{
    _parse(cmd, arg0);
    // free((void *) cmd);
    // free((void *) arg0);
}

void Controller::_parse(char const *cmd0, char const *arg0)
{
    ControllerTargetsMap *map = nullptr;
    std::string arg;
    std::string cmd = cmd0;
    for (ControllerTargetsMap& imap : cmdsMap)
        if (imap.cmd == cmd)
        {
            map = &imap;
        }
    
    if (cmd0)
        free((void *) cmd0);
    if (arg0)
    {
        arg = arg0;
        free((void *) arg0);
    }

    if (map == nullptr)
    {
        /* If we cannot find the command */
        std::ostringstream ss;
        ss << "No such command \'" << cmd << "\'.";
        char const *sug = tryGivingSuggestion(cmd);
        if (sug)
            ss << " Did you mean \'" << sug << "\'?";
        throw ARTException(THROWABLE_THIS, ss.str());
    }
    if (map->expectedArgument && !arg0)
    {
        std::ostringstream ss;
        ss << "Usage: " << map->usage;
        out(ss.str());
        throw ARTException(THROWABLE_THIS, "Command expected an argument but there aren\'t any given arguments");
    }

    double a, b;
    std::ostringstream ss;
    IRDebugger dbg;
    switch (map->identifier)
    {
    case ControllerIdentifier::kTerminate:
        /* throw a logic_error manually to tell main loop to terminate */
        stopLoop();
        break;
    case ControllerIdentifier::kGetMIRLStandard:
        out(MATBAR_MIRL_VER);
        break;
    case ControllerIdentifier::kGetMILStandard:
        out(MATBAR_MIL_VER);
        break;
    case ControllerIdentifier::kGetHelp:
        help(arg);
        break;
    case ControllerIdentifier::kEcho:
        out(getString(arg));
        break;
    case ControllerIdentifier::kGetAuthor:
        out(MATBAR_AUTHOR);
        break;
    case ControllerIdentifier::kExceptionTerminate:
    case ControllerIdentifier::kExceptionDivideByZero:
    case ControllerIdentifier::kDebuggerIRDump:
    case ControllerIdentifier::kDebuggerColor:
        set(map->option, getBoolean(arg));
        break;
    case ControllerIdentifier::kExceptionIgnore:
        set(map->option, !getBoolean(arg));
        break;
    case ControllerIdentifier::kEnumerationVariables:
        variablesEnumerator();
        break;
    case ControllerIdentifier::kEnumerationFunctions:
        functionsEnumerator();
        break;
    case ControllerIdentifier::kEnumerationCommands:
        commandsEnumerator();
        break;
    case ControllerIdentifier::kDebuggerDisassemble:
        dbg.disassembleFunction(arg);
        break;
    case ControllerIdentifier::kLexicalGuessThresold:
        ss.clear();
        ss.setf(std::ios_base::fixed);
        ss.precision(1);
        a = oget(OptionsList::oLexicalGuessThreshold);
        b = getNumber(arg);
        set(map->option, b);
        ss << "LexicalGuessThreshold: " << a << " => " << b;
        out(ss.str());
        break;
    default:
        throw ARTException(THROWABLE_THIS, "Unexpected value for enumeration type ControllerIdentifier");
        break;
    }
}

bool Controller::get(OptionsList ol)
{
    for (OptionsMap& list : optionsMap)
        if (list.opt == ol)
            return list.b_val;
    throw ARTException(THROWABLE_THIS, "OptionList not found");
}

double Controller::oget(OptionsList ol)
{
    for (OptionsMap& list : optionsMap)
        if (list.opt == ol)
            return list.o_val;
    throw ARTException(THROWABLE_THIS, "OptionList not found");
}

void Controller::set(OptionsList ol, bool value)
{
    for (OptionsMap& list : optionsMap)
        if (list.opt == ol)
        {
            list.b_val = value;
            return ;
        }
    throw ARTException(THROWABLE_THIS, "OptionList not found");
}

void Controller::set(OptionsList ol, double value)
{
    for (OptionsMap& list : optionsMap)
        if (list.opt == ol)
        {
            list.o_val = value;
            return ;
        }
    throw ARTException(THROWABLE_THIS, "OptionList not found");
}

void Controller::out(std::string& _str)
{ out(_str.data()); }

void Controller::out(std::string&& _str)
{ out(_str.data()); }

void Controller::out(char const *_str)
{
    std::cout << "\033[32;1m[-]\033[0m\033[1m " << _str << "\033[0m" << std::endl;
}

OptionsMap& Controller::findOption(OptionsList ol)
{
    for (OptionsMap& list : optionsMap)
        if (list.opt == ol)
            return list;
    throw ARTException(THROWABLE_THIS, "OptionList not found");
}

bool Controller::getBoolean(std::string& str)
{
    if (str == "yes" || str == "enable"
        || str == "1" || str == "true")
        return true;
    else if (str == "no" || str == "disable"
            || str == "0" || str == "false")
        return false;
    else
    {
        std::ostringstream ss;
        ss << "getBoolean: Invalid string \'";
        ss << str << "\' for type \'boolean\'";
        throw ARTException(THROWABLE_THIS, ss.str());
    }
}

double Controller::getNumber(std::string& str)
{
    double result = 0;
    std::string intPart, floatPart;
    bool afterPoint = false;

    for (char& ch : str)
    {
        if (ch == '.')
        {
            if (afterPoint)
                goto nerr;
            afterPoint = true;
            continue;
        }
        else if (ch < '0' || ch > '9')
            goto nerr;

        if (afterPoint)
            floatPart.push_back(ch);
        else
            intPart.push_back(ch);
    }

    for (int i = intPart.size()-1, j = 1; i >= 0; i--, j *= 10)
        result += (intPart[i] - '0') * j;
    for (int i = 0, j = 10; i < floatPart.size(); i++, j *= 10)
        result += static_cast<double>(floatPart[i] - '0') / static_cast<double>(j);
    return result;

nerr:
    throw ARTException(THROWABLE_THIS, "getNumber: Invalid string for type 'int' or 'float'");
    /* Will never be executed */
    return -1;
}

std::string Controller::getString(std::string& str)
{
    if (str.front() != '\"' || str.back() != '\"')
        throw ARTException(THROWABLE_THIS, "getString: Invalid string for type \'string\'");
    std::string builder = str;
    /* 掐头去尾（字符串两端有双引号） */
    builder.erase(builder.begin());
    builder.erase(builder.end()-1);
    return builder;
}

void Controller::help(std::string& arg)
{
    for (ControllerTargetsMap& map : cmdsMap)
    {
        if (map.cmd == arg)
        {
            std::ostringstream ss;
            ss << "Usage: " << map.usage;
            out(ss.str());
            out(map.help);
            return ;
        }
    }
    
    std::ostringstream ss;
    ss << "No such command \'" << arg << "\'.";
    char const *sug = tryGivingSuggestion(arg);
    if (sug)
        ss << " Did you mean \'" << sug << "\'?";
    throw ARTException(THROWABLE_THIS, ss.str());
}

char const *Controller::tryGivingSuggestion(std::string& cmd)
{
    size_t len1, len2;
    char const *current;
    char const *ret;
    int **tab;
    int scores[3], minScore;
    double maxRate = -1, rate;

    for (ControllerTargetsMap& map : cmdsMap)
    {
        current = map.cmd;
        len1 = strlen(current);
        len2 = cmd.size();
        tab = allocTable<int>(len1 + 1, len2 + 1);

        for (size_t i = 0; i < len1 + 1; i++)
            tab[i][0] = i;
        for (size_t i = 0; i < len2 + 1; i++)
            tab[0][i] = i;

        for (size_t i = 0; i < len1; i++)
        {
            for (size_t j = 0; j < len2; j++)
            {
                scores[0] = tab[i][j + 1] + 1;
                scores[1] = tab[i + 1][j] + 1;
                if (current[i] == cmd[j])
                    scores[2] = tab[i][j];
                else
                    scores[2] = tab[i][j] + 1;
                minScore = scores[0];
                if (scores[1] < minScore)
                    minScore = scores[1];
                if (scores[2] < minScore)
                    minScore = scores[2];
                tab[i + 1][j + 1] = minScore;
            }
        }

        rate = 1.0 - ((double) tab[len1][len2] / (double)(len1 > len2 ? len1 : len2));
        if (rate > maxRate)
        {
            ret = current;
            maxRate = rate;
        }
        deleteTable<int>(tab, len1 + 1, len2 + 1);
    }

    if (maxRate < oget(OptionsList::oLexicalGuessThreshold))
        return nullptr;
    // std::cout << "rate = " << maxRate << std::endl;
    return ret;
}

template <typename _T>
_T **Controller::allocTable(size_t x, size_t y)
{
    _T **ret = new _T *[x];
    for (int i = 0; i < x; i++)
        ret[i] = new _T[y];
    return ret;
}

template<typename _T>
void  Controller::deleteTable(_T **ptr, size_t x, size_t y)
{
    for (int i = 0; i < x; i++)
        delete[] ptr[i];
    delete[] ptr;
}

void Controller::functionsEnumerator()
{
    VectorEnumerator<IRFunction> ve(vfm.getFunctions());
    ve.newEnumeratable<std::string>("Indentifier", offsetof(IRFunction, identifier));
    ve.newEnumeratable<FunctionTypes>("Type", offsetof(IRFunction, type));
    ve.newEnumeratable<void *>("Address", offsetof(IRFunction, ptr));
    ve.newEnumeratable<AssemblyFunction>("Function Context", offsetof(IRFunction, ctx));
    ve.accept();
}

void Controller::variablesEnumerator()
{
    VectorEnumerator<IRVariable> ve(vfm.getVariables());
    ve.newEnumeratable<std::string>("Identifier", offsetof(IRVariable, identifier));
    ve.newEnumeratable<IRVariableScope>("Scope", offsetof(IRVariable, scope));
    ve.newEnumeratable<double>("Value", offsetof(IRVariable, value));
    ve.accept();
}

void Controller::commandsEnumerator()
{
    VectorEnumerator<ControllerTargetsMap> ve(cmdsMap);
    ve.newEnumeratable<char const *>("Command", offsetof(ControllerTargetsMap, cmd));
    ve.newEnumeratable<char const *>("Usage", offsetof(ControllerTargetsMap, usage));
    ve.newEnumeratable<bool>("Expected Argument", offsetof(ControllerTargetsMap, expectedArgument));
    ve.accept();
}
