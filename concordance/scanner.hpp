#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <map>
#include <list>
#include <print>
#include <string>
#include <string_view>

struct ref
{
    std::string filename;
    int lineno;
    int flags;
};

namespace yy
{
    class Scanner final : public yyFlexLexer
    {
    public:
        int yylex() override final;

    public:
        void set_filename(std::string_view name) { cur_filename = name; }
        void set_lineno(int value) { yylineno = value; }
        void report() const
        {
            for (const auto &[symbol, refs] : symbol_refs)
            {
                std::string_view prev_file;

                std::print("{:15}", symbol);
                for (const auto &ref : refs)
                {
                    if (ref.filename == prev_file)
                    {
                        std::print(" {}", ref.lineno);
                    }
                    else
                    {
                        std::print(" {}:{}", ref.filename, ref.lineno);
                        prev_file = ref.filename;
                    }
                }
                std::println();
            }
        }

    private:
        void add_ref(const std::string &name, std::string filename, int lineno, int flags)
        {
            symbol_refs[name].emplace_back(filename, lineno, flags);
        }

    private:
        std::string cur_filename;
        std::map<std::string, std::list<ref>> symbol_refs;
    };
} // namespace yy
