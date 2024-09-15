#include "scanner.hpp"

#include <print>
#include <fstream>

int main(int argc, char const *argv[])
{
    bool debug_flag = false;

    yy::Scanner wc;
    wc.set_debug(debug_flag);

    if (argc < 2)
    {
        wc.yylex();
        wc.report();
        return 0;
    }

    for (std::size_t i{1}; i < argc; i++)
    {
        std::ifstream file;
        file.open(argv[i]);
        if (file.fail())
        {
            std::println("open file {} failed", argv[i]);
            return 1;
        }
        wc.yyrestart(file);
        wc.yylex();
        wc.report(argv[i]);
        wc.step();
    }

    wc.report_total();

    return 0;
}
