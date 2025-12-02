#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include "Variable.hpp"
#include "TokenStream.hpp"

class Parser
{
public:
    void Run();

private:
    void CleanUp();
    double Expression();
    double Term();
    double Primary();
    double Factor();
    double GetValue(const std::string &s);
    void SetValue(const std::string &s, const double &d);
    double Statement();
    bool IsDeclared(const std::string &var);
    double DefineName(const std::string &var, const double &d);
    double Declaration();

    std::vector<std::unique_ptr<Variable>> variables;
    std::unique_ptr<TokenStream> ts = std::make_unique<TokenStream>();
};

#endif