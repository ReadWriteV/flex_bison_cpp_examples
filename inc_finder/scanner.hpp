#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <print>
#include <stack>
#include <string>
#include <fstream>
#include <string_view>

namespace yy
{
    class Scanner final : public yyFlexLexer
    {
    public:
        int yylex() override final;

        bool enter_file(std::string_view filename);
        bool exit_file();

    private:
        void handle_inc_file();

    private:
        std::stack<std::ifstream> files;
    };
} // namespace yy
