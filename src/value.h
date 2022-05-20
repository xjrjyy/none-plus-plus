#pragma once

#include <string>

#include "token.h"

class Value {
public:
    using NumberType = Calc::NumberType;
    Value(const std::string &name, const NumberType &value)
        : name(name), value(value) {}
    Value(const NumberType &value)
        : name("@unknown"), value(value) {}
    std::string GetName() const { return name; }
    NumberType &GetValue() { return value; }
    NumberType GetValue() const { return value; }
private:
    std::string name;
    NumberType value;
};