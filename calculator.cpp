#include "calculator.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <limits>
#include <cstdlib>
#include <functional>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <utility>
#include <type_traits>

#include "number.h"
#include "value.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"

#include "functions.h"

namespace Calc 
{
Value Calculator::calculate(ExprNodePtr ptr) {
    //std::cout << getTokenName(ptr->type) << " ";
	// function
	if (ptr->type == EToken::Identifier) {
        if (ptr->HasChildren()) {
            ArgsType args;
            for (ExprNodePtr p = ptr->fsn; p != nullptr && !p->IsNothing(); p = p->ne)
                args.push_back(calculate(p).GetValue());
            return Value(GetResultName(ptr->GetIdentifier()), GetFunction(ptr->GetIdentifier())(args));
        } else {
            return Value(ptr->GetIdentifier(), variables[ptr->GetIdentifier()]);
        }
	}
    if (ptr->type == EToken::Equal) {
        if (!ptr->HasChildren()) {
            // TODO: Error
        } else if (ptr->fsn->type != EToken::Identifier && ptr->fsn->type != EToken::Equal) {
            // TODO: GetType
            // TODO: Error
        } else if (ptr->fsn == ptr->lsn) {
            // TODO: Error
        } else if (ptr->fsn->ne != ptr->lsn) {
            // TODO: Error
        } else {
            calculate(ptr->fsn);
            return Value(
                ptr->fsn->GetIdentifier(), 
                variables[ptr->fsn->GetIdentifier()] = calculate(ptr->lsn).GetValue()
            );
        }
    }
	switch (ptr->type) {
	case EToken::Nothing: return Value(NumberType());
	case EToken::Number: return Value(ptr->GetNumber());
	case EToken::Plus: return Value(calculate(ptr->fsn).GetValue() + calculate(ptr->lsn).GetValue());
	case EToken::Sub: return Value(calculate(ptr->fsn).GetValue() - calculate(ptr->lsn).GetValue());
	case EToken::Mul: return Value(calculate(ptr->fsn).GetValue() * calculate(ptr->lsn).GetValue());
	case EToken::Div: return Value(calculate(ptr->fsn).GetValue() / calculate(ptr->lsn).GetValue());
	// TODO:
	case EToken::Identifier:
	// TODO:
	case EToken::Function:
		
	// TODO: Error
	default: 
		return Value(NumberType());
	}
}
Value Calculator::calculate(const std::string &expr) {
	Lexer lexer(expr);
	Parser parser(&lexer);
	ExprNodePtr root(parser.Parse());
	return calculate(root);
}
}