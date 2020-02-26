#include "../include/Enumerator.h"

#define OFFSET(TYPE, MEMBER) ((unsigned long)(&(((TYPE *)0)->MEMBER)))

struct Test
{
    char const *name;
    int age;
    double score;
} __attribute__ ((packed));

int main(int argc, char const *argv[])
{
    
    std::vector<Test> vt = {
        {"Rachel Gardner", 13, 100},
        {"Issac Foster",   20, 90.5},
        {"Jingheng Luo",   14, 92.14159246}
    };
    

    Test test = { "masshiro", 2233, 3.1415 };

    VectorEnumerator<Test> ve(vt);
    ve.newCharEnumeratable("Username", offsetof(Test, name));
    ve.newIntEnumeratable("Ages", offsetof(Test, age));
    ve.newDoubleEnumeratable("Score", offsetof(Test, score));

    ve.accept();
    return 0;
}
