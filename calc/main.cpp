#include "scanner.hpp"
#include "parser.hpp"

#include <print>

int main(int argc, char const *argv[])
{
    bool debug_flag = (argc == 2) && (std::strcmp(argv[1], "-d") == 0);

    yy::Scanner scanner{std::cin, std::cout};
    scanner.set_debug(debug_flag);

    yy::Parser parser{scanner};
    parser.set_debug_level(debug_flag);

    std::print("> ");
    return parser();
}
