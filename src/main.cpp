#include <iostream>
#include <istream>
#include <fstream>
#include <memory>
#include "Parser.hpp"

int main()
{
    Parser parser;
    std::shared_ptr<std::istream> stream;

    try {
        std::string input;

        while (true) {
            std::cout << "Enter 'repl' if you want to use the REPL, but enter a file otherwise: ";
            std::cin >> input;

            if (input == "repl") {
                stream = std::shared_ptr<std::istream>(&std::cin, [](std::istream*){});
                parser.Run(stream);

                break;
            } else {
                auto file_stream = std::make_shared<std::ifstream>(input);

                if (file_stream->is_open()) {
                    stream = file_stream;
                    parser.Run(stream);

                    break;
                } else {
                    std::cout << "Could not open that file!\n";
                }
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred\n";
        return 1;
    }
}