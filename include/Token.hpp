#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <array>
#include <string_view>

constexpr std::array<char, 13> ALL_TOKENS = {
    '(', ')', '{', '}', '*', '%', '/', '+', '-', '!', ';', '.', 'q'
};
constexpr char NUMBER_TOKEN_KIND = '0'; // The "kind" character we use to represent numbers. 
constexpr char QUIT = 'q';
constexpr char PRINT = ';';
constexpr std::string_view PROMPT = "> ";
constexpr std::string_view RESULT = "= ";

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