#include <cstdio>
#include <cstdlib>
#include <readline/readline.h>

char *scannerInteractiveReadline()
{
    char *str = readline("\001\033[34;1m\002>>>\001\033[0m\002 ");
    return str;
}
