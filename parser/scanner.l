%{
#include <Parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define YYERROR_VERBOSE 1

#ifdef __cplusplus
extern "C" {
#endif

#include "y.tab.h"

#ifdef __cplusplus
}
#endif

extern void getInputStream(char *buf, int *result, int max_size);
extern char *scannerInteractiveReadline();
void getInputStream(char *buf, int *result, int max_size)
{
	if ( YY_CURRENT_BUFFER_LVALUE->yy_is_interactive )
	{
		char *str = scannerInteractiveReadline();
		if (!str)
		{
			if (ferror(yyin))
			{
				yy_fatal_error("input in flex scanner failed");
				return ;
			}
			*result = 0;
			return ;
		}

		size_t len = strlen(str);

		for (int i = 0; i < len; i++)
		{
			if (i >= max_size)
			{
				yy_fatal_error("input buffer overflow");
				break;
			}
			buf[i] = str[i];
		}
		free(str);

		if (!len)
		{
			*result = 1;
			buf[0] = '\n';
		}
		else
		{
			*result = len;
		}
	}
	else
	{
		errno = 0;
		while ((*result = (int) fread(buf, 1, (yy_size_t) max_size, yyin)) == 0 && ferror(yyin))
        {
		    if(errno != EINTR)
            {
			    yy_fatal_error("input in flex scanner failed");
				break;
			}
			errno = 0;
			clearerr(yyin);
		}
	}
}

#define YY_INPUT(buf,result,max_size) \
	getInputStream(buf, &result, max_size)

int currentLine = 0;
char buf[32] = "Unrecognized character: ?";
%}

OPERATOR        [-/+*^()\[\]=:;<>{},]
INTEGER         [0-9]+
FLOAT           [0-9]+\.[0-9]+
IDENTIFIER      [_a-zA-Z][_a-zA-Z0-9]*
STRING 			[\"][a-zA-Z0-9~!@#$%^&*()\-+|{}\[\]:;<>,.=\\/?\ ]+[\"]
FUNC_LVAR		\$[_a-zA-Z][_a-zA-Z0-9]*
COMMAND         @[_a-zA-Z-]+
WHITESPACE      [ \t]*
COMMENT			(#|\/\/)

%%
{COMMENT}		{ yy_flush_buffer(YY_CURRENT_BUFFER); {BEGIN INITIAL;} }
{OPERATOR}      { return yytext[0]; }
{INTEGER}       { yylval = strdup(yytext); return T_IntConstant; }
{FLOAT}         { yylval = strdup(yytext); return T_FloatConstant; }
{IDENTIFIER}    { yylval = strdup(yytext); return T_Identifier; }
{STRING}		{ yylval = strdup(yytext); return T_String; }
{FUNC_LVAR}     { yylval = strdup(yytext); return T_FunctionLocalVar; }
{COMMAND}       { yylval = strdup(yytext); return T_Command; }
{WHITESPACE}    { /* ignore every whitespcace */ }
\n              { currentLine++; }
.               { unrecognizedChar(yytext[0]); }
%%

int yywrap(void)
{
    yy_flush_buffer(YY_CURRENT_BUFFER);
    {BEGIN INITIAL;}
    stopLoop();
    return 1;
}

void yyerror(const char *msg)
{
    currentLine = 0;
    yy_flush_buffer(YY_CURRENT_BUFFER);
    {BEGIN INITIAL;}
    highlevelError(msg);
}

void unrecognizedChar(char c)
{
    buf[24] = c;
    yyerror(buf);
}
