#include <iostream>
#include "Parser.hpp"

int main()
{
    Parser parser;

    try
    {
        parser.Run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred\n";
        return 1;
    }
}