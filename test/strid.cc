#include <iostream>
#include <string>

void _moe_get_enumeratable_type(std::string&& __str)
{
    /* 计算字符串的 ID */
    int id = 0;
    std::string::iterator itr = __str.begin();
    for (int i = 1; i <= __str.length(); i++, itr++)
        id += (int)(*itr) * i;
    // id &= __str.length();
    std::cout << id << std::endl;
}

int main(int argc, char const *argv[])
{
    _moe_get_enumeratable_type(argv[1]);
    return 0;
}
