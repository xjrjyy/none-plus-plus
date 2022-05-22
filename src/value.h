#pragma once

#include <string>

#include "token.h"

class Value {
public:
    using NumberType = Calc::NumberType;
    Value(const std::string &, const NumberType &);
    Value(const NumberType &);
    std::string GetName() const;
    NumberType &GetValue();
    NumberType GetValue() const;
private:
    std::string name;
    NumberType value;
};