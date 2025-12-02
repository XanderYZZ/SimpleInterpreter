#include <iostream>
#include "Parser.hpp"

/*

Statement:
    Expression
    Print
    Quit

Print:
    ";"

Quit:
    "q"

Expression:
    Term
    Expression "+" Term
    Expression "-" Term
    
Term:
    Primary
    Term "*" Primary
    Term "/" Primary
    Term "%" Primary

Primary:
    Number
    "(" Expression ")"
    "-" Primary
    "+" Primary

Number:
    double (floating-point-literal)

The input comes from cin through the TokenStream object named "ts."

*/

int main()
{
    Parser parser;

    try {
        parser.Run();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred\n";
        return 1;
    }
}