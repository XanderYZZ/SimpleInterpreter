#include "TokenStream.hpp"
#include <stdexcept>
#include <iostream>
#include <cctype>

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

    if (ALL_TOKENS.contains(ch)) {
        return new Token(ch);
    }

    if (std::isdigit(ch)) {
        std::cin.putback(ch);
        double value = 0;
        std::cin >> value;

        return new Token('0', value);
    }
    
    throw std::runtime_error("Bad token");
}