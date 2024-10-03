#include "scanner.hpp"
#include "parser.hpp"

#include <print>
#include <fstream>

int main(int argc, char const *argv[])
{
    bool debug_flag = (argc >= 2) && (std::strcmp(argv[1], "-d") == 0);

    yy::Scanner scanner{std::cin, std::cout};
    scanner.set_debug(debug_flag);

    std::ifstream file;
    if (argc >= 3)
    {
        file.open(argv[2]);
        if (!file.is_open())
        {
            std::println("open file {} failed", argv[2]);
            return 0;
        }
        scanner.switch_streams(file, std::cout);
    }

    yy::Parser parser{scanner};
    parser.set_debug_level(debug_flag);

    if (parser() == 0)
    {
        std::println("SQL parse worked");
    }
    else
    {
        std::println("SQL parse failed");
    }
    return 0;
}
