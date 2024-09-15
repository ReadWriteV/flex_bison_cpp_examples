#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <print>
#include <string_view>

namespace yy
{
    class Scanner : public yyFlexLexer
    {
    public:
        int yylex() override;

        void report(std::string_view filename = "") const
        {
            std::println("{:8}{:8}{:8} {}", lines, words, chars, filename);
        }

        void report_total() const
        {
            std::println("{:8}{:8}{:8}", total_lines, total_words, total_chars);
        }

        void step()
        {
            total_chars += std::exchange(chars, 0);
            total_words += std::exchange(words, 0);
            total_lines += std::exchange(lines, 1);
        }

    private:
        std::size_t chars{0};
        std::size_t words{0};
        std::size_t lines{1};

        std::size_t total_chars{0};
        std::size_t total_words{0};
        std::size_t total_lines{0};
    };
} // namespace yy