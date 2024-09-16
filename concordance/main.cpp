#include "scanner.hpp"

#include <print>
#include <fstream>

int main(int argc, char const *argv[])
{
    bool debug_flag = false;

    yy::Scanner concordance;
    concordance.set_debug(debug_flag);

    if (argc < 2)
    {
        concordance.set_filename("(std::cin)");
        concordance.yylex();
    }
    else
    {
        for (std::size_t i{1}; i < argc; i++)
        {
            std::ifstream file;
            file.open(argv[i]);
            if (file.fail())
            {
                std::println("open file {} failed", argv[i]);
                return 1;
            }
            concordance.set_filename(argv[i]);
            concordance.set_lineno(1);
            concordance.yyrestart(file);
            concordance.yylex();
        }
    }

    concordance.report();

    return 0;
}
