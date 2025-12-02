#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <iostream>
#include "Token.hpp"
#include "TokenStream.hpp"

class Parser {
public:
    void Run();

private:
    double Expression();
    double Term();
    double Primary();

    std::vector<Token*> tokens;
    TokenStream* ts = new TokenStream();
};

#endif 