#include <RuntimePool.h>
#include <VFManager.h>
#include <stdexcept>
#include <iostream>

#include <Parser.h>

void stopLoop()
{
    std::cout << "\n\033[32;1m[-]\033[0m Program terminated" << std::endl;
    throw std::logic_error("EOP");
}

void highlevelError(char const *err)
{
    throw std::runtime_error(err);
}

void terminator()
{
    std::exception_ptr excp = std::current_exception();
    try {
        if (excp)
            std::rethrow_exception(excp);
    } catch (ARTException& e) {
        std::cerr << "\033[31;1m[!]\033[0m \033[1m" << e.thrower();
        std::cerr << ":\033[0m " << e.msg() << std::endl;
    } catch (std::exception& e) {
        std::cerr << "\033[31;1m[!]\033[0m " << e.what() << std::endl;
    }

    exit(1);
}

int main(int argc, char const *argv[])
{
    std::set_terminate(terminator);

    // std::cout.setf(std::ios_base:);
    std::cout.precision(12);
    
    while (1)
    {
        try {
            yyparse();
        } catch (ARTException& e) {
            std::cerr << "\033[31;1m[!]\033[0m \033[1m" << e.thrower();
            std::cerr << ":\033[0m " << e.msg() << std::endl;
            if (ctl.get(OptionsList::oExceptionTerminate))
                exit(1);
        } catch (std::logic_error& e) {
            break;
        } catch (std::runtime_error& e) {
            std::cerr << "\033[31;1m[!]\033[0m \033[1m" << "Parser";
            std::cerr << ":\033[0m " << e.what() << std::endl;
            if (ctl.get(OptionsList::oExceptionTerminate))
                exit(1);
        }
    }
    return 0;
}
