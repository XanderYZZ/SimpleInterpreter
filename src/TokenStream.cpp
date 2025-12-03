// TokenStream.cpp  (replace the body of the file with this)
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

    buffer = t;
    full = true;
}

std::shared_ptr<Token> TokenStream::GetToken()
{
    if (has_lookahead)
    {
        has_lookahead = false;

        return lookahead;
    }

    if (full)
    {
        full = false;

        return buffer;
    }

    return ReadTokenFromInput();
}

std::shared_ptr<Token> TokenStream::Peek()
{
    if (has_lookahead)
    {
        return lookahead;
    }

    if (full)
    {
        lookahead = buffer;
        has_lookahead = true;

        return lookahead;
    }

    lookahead = ReadTokenFromInput();
    has_lookahead = true;

    return lookahead;
}

std::shared_ptr<Token> TokenStream::ReadTokenFromInput()
{
    int ci;
    do {
        ci = stream->peek();

        if (ci == std::char_traits<char>::eof()) {
            throw std::runtime_error("No more tokens");
        }

        if (std::isspace(static_cast<unsigned char>(ci))) {
            stream->get(); 
            continue;
        }

        break;
    } while (true);

    char ch = static_cast<char>(stream->get()); 

    if (std::find(ALL_TOKENS.begin(), ALL_TOKENS.end(), ch) != ALL_TOKENS.end())
    {
        return std::make_shared<Token>(ch);
    }

    if (std::isdigit(static_cast<unsigned char>(ch)) || ch == '.')
    {
        std::string s;
        s += ch;

        bool has_digit = std::isdigit(ch);

        while (true) {
            int p = stream->peek();
            if (p == std::char_traits<char>::eof()) break;
            char cc = static_cast<char>(p);

            if (std::isdigit(cc) || cc == '.') {
                s += static_cast<char>(stream->get());
                has_digit = true;
            }
            else if ((cc == 'e' || cc == 'E') && has_digit) {
                s += static_cast<char>(stream->get());
                p = stream->peek();
                if (p == '+' || p == '-') s += static_cast<char>(stream->get());
            }
            else break;
        }

        double value = std::stod(s);
        return std::make_shared<Token>(NUMBER_TOKEN_KIND, value);
    }

    if (std::isalpha(static_cast<unsigned char>(ch)) || ch == '_')
    {
        std::string s;
        s += ch;

        while (true) {
            int p = stream->peek();

            if (p == std::char_traits<char>::eof()) break;

            char cc = static_cast<char>(p);

            if (std::isalnum(static_cast<unsigned char>(cc)) || cc == '_') {
                s += static_cast<char>(stream->get());
            } else break;
        }

        if (s == DECLKEY)      return std::make_shared<Token>(LET);
        if (s == DECLCONSTKEY) return std::make_shared<Token>(CONSTLET);
        if (s == SQRT_USER)    return std::make_shared<Token>(SQRT);
        if (s == POW_USER)     return std::make_shared<Token>(POW);

        return std::make_shared<Token>(NAME, s);
    }

    throw std::runtime_error("Bad token");
}

void TokenStream::Ignore(char c)
{
    if (has_lookahead && lookahead && lookahead->GetKind() == c) {
        has_lookahead = false;
        lookahead.reset();

        return;
    }

    if (full && buffer && buffer->GetKind() == c) {
        full = false;
        buffer.reset();

        return;
    }

    while (true) {
        int p = stream->peek();

        if (p == std::char_traits<char>::eof()) {
            return;
        }

        char ch = static_cast<char>(stream->get());

        if (ch == c) return;
    }
}