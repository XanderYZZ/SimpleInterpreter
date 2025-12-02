#include "TokenStream.hpp"
#include <stdexcept>
#include <iostream>

void TokenStream::PushBack(Token* t) {
    if (full) {
        throw std::runtime_error("PushBack() into a full buffer");
    }

    buffer = t;
    full = true;
}

Token* TokenStream::GetToken() {
    if (full) {
        full = false;

        return buffer;
    }

    char ch = 0;
    if (!(std::cin >> ch)) {
        throw std::runtime_error("No more tokens");
    }

    switch (ch) {
        case ';':
        case 'q':
        case '(': case ')': case '+': case '-': case '*': case '/':
            return new Token(ch);
        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            std::cin.putback(ch);
            double value = 0;
            std::cin >> value;

            return new Token('0', value);
        }
        default:
            throw std::runtime_error("Bad token");
    }
}