#include "TokenStream.hpp"
#include <stdexcept>
#include <iostream>
#include <cctype>
#include <algorithm>

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

    if (std::find(ALL_TOKENS.begin(), ALL_TOKENS.end(), ch) != ALL_TOKENS.end()) {
        return new Token(ch);
    }

    if (std::isdigit(ch)) {
        std::cin.putback(ch);
        double value = 0;
        std::cin >> value;

        return new Token(NUMBER_TOKEN_KIND, value);
    }

    if (std::isalpha(ch)) {
        std::string s;
        s += ch;

        while (std::cin.get(ch) && (std::isalpha(ch) || std::isdigit(ch))) {
            s += ch;
        }

        std::cin.putback(ch);

        if (s == DECLKEY) {
            Token *new_token = new Token(LET);

            return new_token;
        } 
        
        Token* new_token = new Token(NAME, s);

        return new_token;
    }
    
    throw std::runtime_error("Bad token");
}

void TokenStream::Ignore(char c) {
    if (full && c == buffer->GetKind()) {
        full = false;

        return;
    }

    full = false;

    char ch = 0;

    while (std::cin >> ch) {
        if (ch == c) { return; }
    }
}