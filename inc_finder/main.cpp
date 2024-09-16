#include "scanner.hpp"

#include <print>

int main(int argc, char const *argv[])
{
    bool debug_flag = false;

    yy::Scanner inc_finder;
    inc_finder.set_debug(debug_flag);

    if (argc < 2)
    {
        std::println("need filename");
        return 0;
    }

    if (inc_finder.enter_file(argv[1]))
    {
        inc_finder.yylex();
    }

    return 0;
}
