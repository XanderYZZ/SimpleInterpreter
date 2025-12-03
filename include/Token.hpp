#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <array>
#include <string_view>
#include <string>

constexpr std::array<char, 15> ALL_TOKENS = {
    '(', ')', '{', '}', '*', '%', '/', '+', '-', '!', ';', '.', '=', 'q', ','};
constexpr char NUMBER_TOKEN_KIND = '0'; // The "kind" character we use to represent numbers.
constexpr char QUIT = 'q';
constexpr char PRINT = ';';
constexpr std::string_view PROMPT = "> ";
constexpr std::string_view RESULT = "= ";
constexpr std::string_view DECLKEY = "let";
constexpr std::string_view SQRT_USER = "sqrt";
constexpr std::string_view POW_USER = "pow";
constexpr char NAME = 'A';
constexpr char LET = 'L';
constexpr char SQRT = 'S';
constexpr char POW = 'P';
constexpr char ASSIGNMENT = '=';

class Token
{
public:
    Token(const char &kind) : kind(kind), value(0) {}
    Token(const char &kind, const double &value) : kind(kind), value(value) {}
    Token(const char &kind, const std::string &name) : kind(kind), value(0), name(name) {}
    char GetKind() const { return kind; }
    double GetValue() const { return value; }
    std::string GetName() const { return name; }

private:
    char kind;
    double value;
    std::string name;
};

#endif