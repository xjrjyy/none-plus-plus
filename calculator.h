#pragma once

#include <string>
#include <unordered_map>

#include "token.h"
#include "value.h"
#include "lexer.h"
#include "parser.h"

namespace Calc
{
class Calculator {
public:
    Calculator() {}
    ~Calculator() {}
    static std::string GetResultName(const std::string &function_name) {
        return kSpecialPrefix + function_name + std::string("_result");
    }
    Value calculate(ExprNodePtr);
    Value calculate(const std::string &);
private:
    std::unordered_map<std::string, NumberType> variables;
};
}