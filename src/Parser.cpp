#include <iostream>
#include <cmath>
#include "Parser.hpp"

void Parser::Run() {
    double value = 0;

    while (std::cin) {
        try {
            std::cout << PROMPT;
            Token *t = ts->GetToken();

            while (t->GetKind() == PRINT) {
                t = ts->GetToken();
            }

            if (t->GetKind() == QUIT) {
                return;
            }

            ts->PushBack(t);
            std::cout << RESULT << Statement() << "\n";
        } catch (std::exception &e) {
            std::cerr << e.what() << "\n";
            CleanUp();
        }
    }
}

void Parser::CleanUp() {
    ts->Ignore(PRINT);
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

        case NUMBER_TOKEN_KIND:
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

// Return the value of the Variable that has the name equal to the argument "s."
double Parser::GetValue(const std::string &s) {
    for (Variable *v : variables) {
        if (v->GetName() == s) { return v->GetValue(); }
    }

    throw std::runtime_error("Trying to read from undefined variable " + s);
}

// Set the variable with the name equal to the argument "s" to have the value equal to the argument "d."
void Parser::SetValue(const std::string &s, const double &d) {
    for (Variable *v : variables) {
        if (v->GetName() == s) {
            v->SetValue(d);
            
            return;
        }
    }

    throw std::runtime_error("Trying to write to undefined variable " + s);
}

double Parser::Statement() {
    Token *t = ts->GetToken();

    switch(t->GetKind()) {
        case LET:
            return Declaration();
        default:
            ts->PushBack(t);

            return Expression();
    }
}

bool Parser::IsDeclared(const std::string &var) {
    for (Variable *v : variables) {
        if (v->GetName() == var) { return true; }
    }

    return false;
}

double Parser::DefineName(const std::string &var, const double &d) {
    if (IsDeclared(var)) {
        throw std::runtime_error(var + " is declared twice");
    }

    variables.push_back(&Variable(var, d));

    return d;
}

double Parser::Declaration() {
    Token *t = ts->GetToken();

    if (t->GetKind() != NAME) {
        throw std::runtime_error("name expected in declaration");
    }

    Token *t2 = ts->GetToken();

    if (t2->GetKind() != '=') {
        throw std::runtime_error("= missing in declaration of " + t->GetKind());
    }

    double d = Expression();
    DefineName(t->GetName(), d);

    return d;
}