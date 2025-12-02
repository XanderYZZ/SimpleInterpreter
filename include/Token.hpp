#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <unordered_set>

const std::unordered_set<char> ALL_TOKENS = {
    '(', ')', '{', '}', '*', '%', '/', '+', '-', '!', ';', '.', 'q'
};

class Token {
public:
    Token(char kind) : kind(kind), value(0) {}
    Token(char kind, double value) : kind(kind), value(value) {}
    char GetKind() const { return kind; }
    double GetValue() const { return value; }

private:
    char kind;
    double value;
};

#endif