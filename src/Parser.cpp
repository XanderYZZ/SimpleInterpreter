#include "Parser.hpp"

void Parser::Run() {
    double value = 0;

    while (std::cin) {
        Token *t = ts->GetToken();

        if (t->GetKind() == 'q') {
            break;
        } else if (t->GetKind() == ';') {
            std::cout << "=" << value << "\n";
        } else {
            ts->PushBack(t);
        }

        value = Expression();
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

double Parser::Term() {
    double left = Primary();
    Token *t = ts->GetToken();

    while (true) {
        switch(t->GetKind()) {
            case '*':
                left *= Primary();
                t = ts->GetToken();
                break;
            case '/': {
                double divisor = Primary();

                if (divisor == 0) {
                    std::cerr << "Error: Division by zero\n";
                    return 0;
                }

                left /= divisor;
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
        case '(': {
            double d = Expression();
            t = ts->GetToken();

            if (t->GetKind() != ')') {
                std::cerr << "Error: Expected ')'\n";
                return 0;
            }

            return d;
        }

        case '0':
            return t->GetValue();
        default:
            std::cerr << "Error: Primary expected\n";
            return 0;
    }
}