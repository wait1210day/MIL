%{
#include <RuntimePool.h>
#include <Parser.h>
#define YYERROR_VERBOSE 1
%}

%token T_IntConstant
%token T_FloatConstant
%token T_Identifier
%token T_Command
%token T_FunctionLocalVar
%token T_String
// %token T_FunctionDef

%left '+' '-'
%left '*' '/'
%left '^'
%right U_neg

%start S

%%

S	:	Stmt
    |   S Stmt
    ;

Stmt:	T_Command T_Identifier     ';'  { ctl.parse($1, $3);  }
    |   T_Command T_Command        ';'  { ctl.parse($1, $3);  }
    |   T_Command T_IntConstant    ';'  { ctl.parse($1, $3);  }
    |   T_Command T_FloatConstant  ';'  { ctl.parse($1, $3);  }
    |   T_Command T_String         ';'  { ctl.parse($1, $3);  }
    |   T_Command ';'                   { ctl.parse($1);                    }
    |   T_Identifier '=' E ';'          { codeGen.pop($1); codeGen.final(); }
    |   E ';'                           { codeGen.drop(); codeGen.final();  }
    |   T_Identifier '[' T_FunctionLocalVar ']' '=' E ';'
                                        { codeGen.def($1, $3); codeGen.final(); }
    ;

E   :   E '+' E                     { codeGen.add(); }
    |   E '-' E                     { codeGen.sub(); }
    |   E '*' E                     { codeGen.mul(); }
    |   E '/' E                     { codeGen.div(); }
    |   E '^' E                     { codeGen.pow(); }
    |   '-' E %prec U_neg           { codeGen.neg(); }
    |   T_Identifier '(' E ')'      { codeGen.call($1); }
    |   T_IntConstant               { codeGen.pushInt($1); }
    |   T_FloatConstant             { codeGen.pushFloat($1); }
    |   T_Identifier                { codeGen.pushVariable($1); }
    |   '(' E ')'                   { /* empty */ }
    ;

%%
