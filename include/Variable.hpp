#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <string>

class Variable {
public:
    Variable(const std::string &name, const double &value) : name(name), value(value) {};
    std::string GetName() { return name; }
    double GetValue() { return value; }
    void SetValue(const double &value) { this->value = value; }

private:
    std::string name;
    double value;
};

#endif