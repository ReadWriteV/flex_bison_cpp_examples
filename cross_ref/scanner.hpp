#pragma once

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <map>
#include <list>
#include <print>
#include <stack>
#include <string>
#include <fstream>
#include <string_view>

struct ref
{
    std::string filename;
    int lineno;
    int flags;
};

struct file_state
{
    std::string name;
    std::ifstream file;
    int lineno;
};

namespace yy
{
    class Scanner final : public yyFlexLexer
    {
    public:
        int yylex() override final;

        bool enter_file(std::string_view filename);
        bool exit_file();

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
                    if (ref.flags & 0x1)
                    {
                        std::print("*");
                    }
                }
                std::println();
            }
        }

    private:
        void handle_inc_file();

        void add_ref(const std::string &name, std::string filename, int lineno, int flags)
        {
            symbol_refs[name].emplace_back(filename, lineno, flags);
        }

    private:
        bool defining;
        std::string cur_filename;
        std::stack<file_state> file_states;
        std::map<std::string, std::list<ref>> symbol_refs;
    };
} // namespace yy
