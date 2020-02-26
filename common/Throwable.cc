#include <Throwable.h>

Throwable::Throwable(char const *name)
{
    __thrower = name;
}

std::string& Throwable::getThrower()
{
    return __thrower;
}

ARTException::ARTException(Throwable& cl, const std::string& msg)
{
    __thrower = cl.getThrower();
    __msg = msg;
}

ARTException::ARTException(Throwable& cl, const std::string&& msg)
{
    __thrower = cl.getThrower();
    __msg = msg;
}

std::string ARTException::what()
{
    return (__thrower + ": " + __msg);
}

std::string& ARTException::thrower()
{
    return __thrower;
}

std::string& ARTException::msg()
{
    return __msg;
}
