#include "scanner.hpp"

#include <print>

int main(int argc, char const *argv[])
{
    bool debug_flag = false;

    yy::Scanner cross_referencer;
    cross_referencer.set_debug(debug_flag);

    if (argc < 2)
    {
        std::println("need filename");
        return 0;
    }

    for (std::size_t i{1}; i < argc; i++)
    {
        if (cross_referencer.enter_file(argv[i]))
        {
            cross_referencer.yylex();
        }
    }

    cross_referencer.report();

    return 0;
}
