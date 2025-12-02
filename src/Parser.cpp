#include <iostream>
#include <cmath>
#include "Parser.hpp"

void Parser::Run() {
    double value = 0;

    while (std::cin) {
        std::cout << "> ";
        Token *t = ts->GetToken();

        while (t->GetKind() == ';') {
            t = ts->GetToken();
        }

        if (t->GetKind() == 'q') {
            return;
        }

        ts->PushBack(t);
        std::cout << "=" << Expression() << "\n";
    }
}

double Parser::Expression() {
    double left = Term();
    Token *t = ts->GetToken();

    while (true) {
        switch(t->GetKind()) {
            case '+':
                left += Term();
                t = ts->GetToken();

                break;
            case '-':
                left -= Term();
                t = ts->GetToken();

                break;
            default:
                ts->PushBack(t);

                return left;
        }
    }
}

double Parser::Factor() {
    double left = Primary();
    Token *t = ts->GetToken();

    while (true) {
        switch(t->GetKind()) {
            case '!': {
                int factorial = 1;
                int upper = static_cast<int>(left);

                for (int i = 1; i <= upper; i++) {
                    factorial *= i;
                }

                left = factorial;
                t = ts->GetToken();

                break;
            }
            default:
                ts->PushBack(t);

                return left;
        }
    }
}

double Parser::Term() {
    double left = Factor();
    Token *t = ts->GetToken();

    while (true) {
        switch(t->GetKind()) {
            case '*':
                left *= Factor();
                t = ts->GetToken();

                break;
            case '/': {
                double divisor = Factor();

                if (divisor == 0) {
                    std::cerr << "Error: Division by zero\n";
                    return 0;
                }

                left /= divisor;
                t = ts->GetToken();

                break;
            }
            case '%': {
                double divisor = Primary();

                if (divisor == 0) {
                    std::cerr << "Error: Division by zero\n";
                    return 0;
                }

                left = std::fmod(left, divisor);
                t = ts->GetToken();

                break;
            }
            default:
                ts->PushBack(t);

                return left;
        }
    }
}

double Parser::Primary() {
    Token *t = ts->GetToken();

    switch(t->GetKind()) {
        case '(': case '{': {
            double d = Expression();
            t = ts->GetToken();

            if (t->GetKind() != ')' && t->GetKind() != '}') {
                std::cerr << "Error: Expected ')'\n";
                return 0;
            }

            return d;
        }

        case '0':
            return t->GetValue();
        case '-':
            return -Primary();
        case '+':
            return Primary();
        default:
            std::cerr << "Error: Primary expected\n";

            return 0;
    }
}