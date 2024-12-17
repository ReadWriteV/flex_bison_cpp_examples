#include "ast.hpp"

#include <cmath>
#include <print>
#include <algorithm>
#include <unordered_map>

static std::unordered_map<std::string, std::unique_ptr<Symbol>> symbols;

double Node::eval() const
{
    using enum type;
    switch (op)
    {
    case add:
        return left->eval() + right->eval();
    case sub:
        return left->eval() - right->eval();
    case mul:
        return left->eval() * right->eval();
    case div:
        return left->eval() / right->eval();
    case abs:
        return std::abs(left->eval());
    case minus:
        return -left->eval();
    case lt:
        return left->eval() < right->eval();
    case lte:
        return left->eval() <= right->eval();
    case gt:
        return left->eval() > right->eval();
    case gte:
        return left->eval() >= right->eval();
    case eq:
        return left->eval() == right->eval();
    case neq:
        return left->eval() != right->eval();
    case list:
        return (left->eval(), right->eval());
    default:
        return 0.0;
    }
    std::unreachable();
}

double FnCall::eval() const
{
    auto v = arg->eval();
    using enum builtin_func;
    switch (functype)
    {
    case sqrt:
        return std::sqrt(v);
    case exp:
        return std::exp(v);
    case log:
        return std::log(v);
    case print:
        std::println(" = {:4.4f}", v);
        return v;
    default:
        std::unreachable();
    }
}

double UFnCall::eval() const
{
    auto fn = get_symbol(name);
    auto real_args = args.get();

    if (!fn->func)
    {
        std::println("call to undefined function");
        return 0.0;
    }

    const auto num_of_args = fn->args.size();

    std::vector<double> old_values, new_values;
    old_values.resize(num_of_args);
    new_values.resize(num_of_args);

    for (size_t i = 0; i < num_of_args; i++)
    {
        if (!real_args)
        {
            std::println("to few args");
            return 0.0;
        }
        if (auto node = dynamic_cast<Node *>(real_args); node && node->op == Node::type::list)
        {
            new_values[i] = node->left->eval();
            real_args = node->right.get();
        }
        else
        {
            new_values[i] = real_args->eval();
            real_args = nullptr;
        }
    }

    const auto &virtual_args = fn->args;

    auto iter = virtual_args.begin();

    for (size_t i = 0; i < num_of_args; i++)
    {
        old_values.at(i) = (*iter)->value;
        (*iter)->value = new_values.at(i);
        iter++;
    }

    auto v = fn->func->eval();

    iter = virtual_args.begin();

    for (size_t i = 0; i < num_of_args; i++)
    {
        (*iter)->value = old_values.at(i);
        iter++;
    }

    return v;
}

double Branch::eval() const
{
    if (cond->eval() != 0)
    {
        return then_br ? then_br->eval() : 0.0;
    }
    else
    {
        return else_br ? else_br->eval() : 0.0;
    }
}

double Loop::eval() const
{
    double v = 0.0;
    if (cond)
    {
        while (cond->eval())
        {
            v = body->eval();
        }
    }
    return v;
}

void make_def(std::string name, symbol_list_type symlist, ast_type body)
{
    symbols.insert_or_assign(name, std::make_unique<Symbol>(0.0, std::move(body), std::move(symlist)));
}

Symbol *get_symbol(std::string name)
{
    auto iter = symbols.find(name);
    if (iter != symbols.end())
    {
        return iter->second.get();
    }
    iter = symbols.emplace(name, std::make_unique<Symbol>()).first;
    return iter->second.get();
}

ast_type make_node(Node::type op, ast_type left, ast_type right)
{
    if (right == nullptr)
    {
        return std::move(left);
    }
    return std::make_unique<Node>(op, std::move(left), std::move(right));
}

ast_type make_func(FnCall::builtin_func type, ast_type arg)
{
    return std::make_unique<FnCall>(type, std::move(arg));
}

ast_type make_call(std::string name, ast_type arg)
{
    return std::make_unique<UFnCall>(std::move(name), std::move(arg));
}

ast_type make_branch(ast_type cond, ast_type then_br, ast_type else_br)
{
    return std::make_unique<Branch>(std::move(cond), std::move(then_br), std::move(else_br));
}

ast_type make_loop(ast_type cond, ast_type body)
{
    return std::make_unique<Loop>(std::move(cond), std::move(body));
}

ast_type make_number(double v)
{
    return std::make_unique<Number>(v);
}

ast_type make_ref(std::string name)
{
    return std::make_unique<SymRef>(std::move(name));
}

ast_type make_asgn(std::string name, ast_type src)
{
    return std::make_unique<SymAsgn>(std::move(name), std::move(src));
}

symbol_list_type make_symlist(std::string name, symbol_list_type l)
{
    l.push_front(get_symbol(name));
    return l;
}
