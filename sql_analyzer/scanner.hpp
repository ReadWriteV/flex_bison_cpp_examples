#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.hpp"

namespace yy
{
    class Scanner : public yyFlexLexer
    {
    public:
        using yyFlexLexer::yyFlexLexer;

        void initialize_location(yy::location::filename_type &f, yy::location::counter_type l = 1, yy::location::counter_type c = 1)
        {
            loc.initialize(&f, l, c);
        }

        yy::Parser::symbol_type scan();

    private:
        yy::location loc;
    };
} // namespace yy

#undef YY_DECL
#define YY_DECL yy::Parser::symbol_type yy::Scanner::scan()
