#include <iostream>
#include <cmath>
#include "Parser.hpp"

void Parser::Run()
{
    DefineName("pi", 3.1415926535, true);
    DefineName("e", 2.7182818284, true);
    double value = 0;

    while (std::cin)
    {
        try
        {
            std::cout << PROMPT;
            auto t = ts->GetToken();

            while (t->GetKind() == PRINT)
            {
                t = ts->GetToken();
            }

            if (t->GetKind() == QUIT)
            {
                return;
            }

            ts->PushBack(t);
            std::cout << RESULT << Statement() << "\n";
            auto end = ts->GetToken();

            if (end->GetKind() != PRINT) {
                throw std::runtime_error("';' expected");
            }
        }
        catch (std::exception &e)
        {
            std::cerr << e.what() << "\n";
            CleanUp();
        }
    }
}

void Parser::CleanUp()
{
    ts->Ignore(PRINT);
}

double Parser::Expression(std::shared_ptr<Token> first)
{
    double left = first ? Term(first) : Term();
    auto t = ts->GetToken();

    while (true)
    {
        switch (t->GetKind())
        {
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

double Parser::ExpressionUntil(const char &c) {
    auto t = ts->GetToken();

    if (t->GetKind() == c) {
        return 0; 
    }

    ts->PushBack(t);
    double val = Expression();

    return val;
}

double Parser::Factor(std::shared_ptr<Token> first)
{
    double left = first ? Primary(first) : Primary();

    while (true)
    {
        auto t = ts->GetToken();

        switch (t->GetKind())
        {
        case '!':
        {
            int factorial = 1;
            int upper = static_cast<int>(left);

            for (int i = 1; i <= upper; i++)
            {
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

double Parser::Term(std::shared_ptr<Token> first) {
    double left = first ? Factor(first) : Factor();

    while (true) {
        auto t = ts->GetToken();

        switch (t->GetKind()) {
        case '*':
                left *= Factor();
                t = ts->GetToken();

                break;
        case '/':
            {
                double divisor = Factor();

                if (divisor == 0)
                {
                    std::cerr << "Error: Division by zero\n";
                    return 0;
                }

                left /= divisor;
                t = ts->GetToken();

                break;
            }
        case '%':
            {
                double divisor = Primary();

                if (divisor == 0)
                {
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

double Parser::Primary(std::shared_ptr<Token> first)
{
    auto t = first ? first : ts->GetToken();

    switch (t->GetKind())
    {
    case '(':
    case '{':
    {
        double d = Expression();
        t = ts->GetToken();

        if (t->GetKind() != ')' && t->GetKind() != '}')
        {
            std::cerr << "Error: Expected ')'\n";
            return 0;
        }

        return d;
    }

    case NUMBER_TOKEN_KIND:
        return t->GetValue();
    case NAME:
        return GetValue(t->GetName());
    case SQRT: {
        double to_use = Primary();

        if (to_use < 0) {
            throw std::runtime_error("You cannot take the square root of a negative number!");
        }
        
        return std::sqrt(to_use);
    }
    case POW: {
        auto t = ts->GetToken();

        if (t->GetKind() != '(') {
            throw std::runtime_error("Expected '(' after pow");
        }

        double base = ExpressionUntil(','); 
        t = ts->GetToken();

        if (t->GetKind() != ',') {
            throw std::runtime_error("Expected ',' in pow");
        }

        double exponent = ExpressionUntil(')');
        t = ts->GetToken();

        if (t->GetKind() != ')') {
            throw std::runtime_error("Expected ')' after pow");
        }

        return std::pow(base, exponent);
    }
    case '-':
        return -Primary();
    case '+':
        return Primary();
    default:
        std::cerr << "Debug: Primary got kind '" << t->GetKind() << "' (" << static_cast<int>(t->GetKind()) << ") name: '" << t->GetName() << "'\n";
        std::cerr << "Error: Primary expected\n";

        return 0;
    }
}

// Return the value of the Variable that has the name equal to the argument "s."
double Parser::GetValue(const std::string &s)
{
    for (std::unique_ptr<Variable> &v : variables)
    {
        if (v->GetName() == s)
        {
            return v->GetValue();
        }
    }

    throw std::runtime_error("Trying to read from undefined variable " + s);
}

// Set the variable with the name equal to the argument "s" to have the value equal to the argument "d."
void Parser::SetValue(const std::string &s, const double &d)
{
    for (std::unique_ptr<Variable> &v : variables)
    {
        if (v->GetName() == s)
        {
            v->SetValue(d);

            return;
        }
    }

    throw std::runtime_error("Trying to write to undefined variable " + s);
}

double Parser::Statement()
{
    auto t = ts->GetToken();

    switch (t->GetKind())
    {
    case LET:
        return Declaration(false);
    case CONSTLET:
        return Declaration(true);
    case NAME: {
        auto next = ts->Peek();

        if (next->GetKind() == ASSIGNMENT) {
            ts->GetToken();
            double value = Expression();
            SetValue(t->GetName(), value);

            return value;
        }

        return Expression(t);
    }
    default:
        ts->PushBack(t);

        return Expression();
    }
}

double Parser::Declaration(const bool &is_constant)
{
    auto t = ts->GetToken();

    if (t->GetKind() != NAME)
    {
        throw std::runtime_error("name expected in declaration");
    }

    auto t2 = ts->GetToken();

    if (t2->GetKind() != '=')
    {
        throw std::runtime_error("= missing in declaration");
    }

    double d = Expression();
    DefineName(t->GetName(), d, is_constant);

    return d;
}

bool Parser::IsDeclared(const std::string &var)
{
    for (std::unique_ptr<Variable> &v : variables)
    {
        if (v->GetName() == var)
        {
            return true;
        }
    }

    return false;
}

double Parser::DefineName(const std::string &var, const double &d, const bool &is_constant)
{
    if (IsDeclared(var))
    {
        throw std::runtime_error(var + " is declared twice");
    }

    variables.push_back(std::make_unique<Variable>(Variable(var, d, is_constant)));

    return d;
}