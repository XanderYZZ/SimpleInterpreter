#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include <istream>
#include "Variable.hpp"
#include "TokenStream.hpp"

class Parser
{
public:
    void Run(std::shared_ptr<std::istream> stream);

private:
    void CleanUp();
    double Expression(std::shared_ptr<Token> first = nullptr);
    double Term(std::shared_ptr<Token> first = nullptr);
    double Primary(std::shared_ptr<Token> first = nullptr);
    double Factor(std::shared_ptr<Token> first = nullptr);
    double GetValue(const std::string &s);
    void SetValue(const std::string &s, const double &d);
    double Statement();
    bool IsDeclared(const std::string &var);
    double DefineName(const std::string &var, const double &d, const bool &is_constant);
    double Declaration(const bool &is_constant);

    std::vector<std::unique_ptr<Variable>> variables;
    std::unique_ptr<TokenStream> ts;
};

#endif