#pragma once

#include <list>
#include <string>
#include <memory>

struct Value
{
    virtual ~Value() = default;
    virtual double eval() const = 0;
};

using ast_type = std::unique_ptr<Value>;

struct Symbol
{
    double value;

    ast_type func;
    std::list<Symbol *> args;
};

using symbol_list_type = std::list<Symbol *>;

Symbol *get_symbol(std::string name);

struct Node final : public Value
{
    enum class type
    {
        add,
        sub,
        mul,
        div,
        abs,
        minus,

        lt,
        lte,
        gt,
        gte,
        eq,
        neq,

        list,
    };

    Node(type op, ast_type left, ast_type right) : op{op}, left{std::move(left)}, right{std::move(right)} {}

    double eval() const override;

    type op;
    ast_type left;
    ast_type right;
};

struct FnCall final : public Value
{
    enum class builtin_func
    {
        sqrt,
        exp,
        log,
        print
    };

    FnCall(builtin_func functype, ast_type arg) : functype{functype}, arg{std::move(arg)} {}

    double eval() const override;

    builtin_func functype;
    ast_type arg;
};

struct UFnCall final : public Value
{
    UFnCall(std::string name, ast_type args) : name{std::move(name)}, args{std::move(args)} {}

    double eval() const override;

    std::string name;
    ast_type args;
};

struct Branch final : public Value
{
    Branch(ast_type cond, ast_type then_br, ast_type else_br) : cond{std::move(cond)}, then_br{std::move(then_br)}, else_br{std::move(else_br)} {}

    double eval() const override;

    ast_type cond;
    ast_type then_br;
    ast_type else_br;
};

struct Loop final : public Value
{
    Loop(ast_type cond, ast_type body) : cond{std::move(cond)}, body{std::move(body)} {}
    double eval() const override;

    ast_type cond;
    ast_type body;
};

struct Number final : public Value
{
    Number(double v) : v{v} {}
    double eval() const override { return v; }

    double v;
};

struct SymRef final : public Value
{
    SymRef(std::string name) : name{std::move(name)} {}
    double eval() const override { return get_symbol(name)->value; }

    std::string name;
};

struct SymAsgn final : public Value
{
    SymAsgn(std::string name, ast_type v) : name{std::move(name)}, v{std::move(v)} {}
    double eval() const override
    {
        auto s = get_symbol(name);
        s->value = v->eval();
        return s->value;
    }

    std::string name;
    ast_type v;
};

void make_def(std::string name, symbol_list_type symlist, ast_type body);

ast_type
make_node(Node::type op, ast_type left, ast_type right = nullptr);

ast_type
make_func(FnCall::builtin_func type, ast_type arg);

ast_type
make_call(std::string name, ast_type arg);

ast_type
make_branch(ast_type cond, ast_type then_br, ast_type else_br = nullptr);

ast_type
make_loop(ast_type cond, ast_type body);

ast_type make_number(double v);

ast_type make_ref(std::string name);

ast_type make_asgn(std::string name, ast_type src);

symbol_list_type make_symlist(std::string name, symbol_list_type l = {});
