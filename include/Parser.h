#ifndef __PARSER_H__
#define __PARSER_H__

extern int currentLine;
extern char *yytext;
#define YYSTYPE char *

#ifdef __cplusplus
extern "C" {
#endif

int yywrap(void);
int yylex(void);
int yyparse();
void yyerror(const char *msg);
void unrecognizedChar(char c);
void highlevelError(char const *);
void stopLoop(void);

#ifdef __cplusplus
}
#endif

#endif // __PARSER_H__
