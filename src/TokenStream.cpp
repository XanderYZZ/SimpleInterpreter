#include "TokenStream.hpp"
#include <stdexcept>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <memory>

void TokenStream::PushBack(std::shared_ptr<Token> t)
{
    if (full)
    {
        throw std::runtime_error("PushBack() into a full buffer");
    }

    buffer = std::move(t);
    full = true;
}

std::shared_ptr<Token> TokenStream::GetToken()
{
    if (full)
    {
        full = false;

        return std::move(buffer);
    }

    char ch = 0;
    if (!(std::cin >> ch))
    {
        throw std::runtime_error("No more tokens");
    }

    if (std::find(ALL_TOKENS.begin(), ALL_TOKENS.end(), ch) != ALL_TOKENS.end())
    {
        return std::make_shared<Token>(ch);
    }

    if (std::isdigit(ch))
    {
        std::cin.putback(ch);
        double value = 0;
        std::cin >> value;

        return std::make_shared<Token>(NUMBER_TOKEN_KIND, value);
    }

    if (std::isalpha(ch))
    {
        std::string s;
        s += ch;

        while (std::cin.get(ch) && (std::isalpha(ch) || std::isdigit(ch) || ch == '_'))
        {
            s += ch;
        }

        std::cin.putback(ch);

        if (s == DECLKEY) {
            return std::make_shared<Token>(LET);
        } else if (s == SQRT_USER) {
            return std::make_shared<Token>(SQRT);
        } else if (s == POW_USER) {
            std::shared_ptr<Token> first = GetToken();

            if (first->GetKind() != '(') {
                throw std::runtime_error("We need an opening parenthesis!");
            }

            double second = 0;
            std::cin >> second;
            char non_num = 0;
            std::cin >> non_num;
            double third = 0;
            std::cin >> third;
            std::cin >> non_num;

            return std::make_shared<Token>(POW, std::pow(second, third));
        }

        return std::make_shared<Token>(NAME, s);
    }

    throw std::runtime_error("Bad token");
}

void TokenStream::Ignore(char c)
{
    if (full && c == buffer->GetKind())
    {
        full = false;

        return;
    }

    full = false;

    char ch = 0;

    while (std::cin >> ch)
    {
        if (ch == c)
        {
            return;
        }
    }
}