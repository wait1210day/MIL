#ifndef __THROWABLE_H__
#define __THROWABLE_H__

#include <string>
// #include <stdexcept>
#define THROWABLE_CLASS(name) Throwable(name)
#define THROWABLE_THIS *this

class Throwable
{
public:
    Throwable(char const *name);
    std::string& getThrower();

private:
    std::string __thrower;
};

class ARTException
{
public:
    ARTException(Throwable& cl, const std::string& msg);
    ARTException(Throwable& cl, const std::string&& msg);
    std::string what();
    std::string& thrower();
    std::string& msg();

private:
    std::string __thrower;
    std::string __msg;
};

#endif /* __THROWABLE_H__ */
