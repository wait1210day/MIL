#include <stdio.h>

int main(int argc, char const *argv[])
{
    for (char ch = 'a'; ch <= 'z'; ch++)
        printf("%c = %d;\n", ch, ch - 'a');
    for (int i = 1; i < 26; i++)
    {
        printf("func%d[$x] = ", i);
        for (int j = 0; j <= i; j++)
        {
            if (j != 0)
                printf("+");
            if (i == j)
                printf("%c", 'a' + j);
            else
                printf("%c*x^%d", 'a' + j, i - j);
        }
        printf(";\n");
    }
    puts("@enumeration-functions;\n");
    for (int i = 25; i > 0; i--)
    {
        printf("func%d(%d);\n", i, 26 - i);
    }

    for (char c1 = 'a'; c1 <= 'z'; c1++)
        for (char c2 = 'a'; c2 <= 'z'; c2++)
            for (char c3 = 'a'; c3 <= 'z'; c3++)
                printf("%c%c%c = %d;\n", c1, c2, c3, (c1 - 'a') + (c2 - 'a') + (c3 - 'a'));
    for (char c1 = 'a'; c1 <= 'z'; c1++)
        for (char c2 = 'a'; c2 <= 'z'; c2++)
            for (char c3 = 'a'; c3 <= 'z'; c3++)
                printf("%c%c%c+", c1, c2, c3);
    printf("1;\n");
    return 0;
}
