#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <string>

class Variable {
public:
    Variable(const std::string &name, const double &value) : name(name), value(value) {};
    Variable(const std::string &name, const double &value, const bool &is_constant) : name(name), value(value), is_constant(is_constant) {}
    std::string GetName() { return name; }
    double GetValue() { return value; }
    void SetValue(const double &value) {
        if (is_constant) {
            std::cerr << "\nYou cannot change a constant!\n";

            return;
        }

        this->value = value;
    }

private:
    std::string name;
    double value;
    bool is_constant = false;
};

#endif