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
	if (ptr->type == tok::Identifier) {
        if (ptr->HasChildren()) {
            ArgsType args;
            for (ExprNodePtr p = ptr->fsn; p != nullptr && !p->IsNothing(); p = p->ne)
                args.push_back(calculate(p).GetValue());
            return Value(GetResultName(ptr->GetIdentifier()), GetFunction(ptr->GetIdentifier())(args));
        } else {
            return Value(ptr->GetIdentifier(), variables[ptr->GetIdentifier()]);
        }
	}
    if (IsAssignToken(ptr->type)) {
        if (!ptr->HasChildren()) {
            // TODO: Error
        } else if (ptr->fsn->type != tok::Identifier && ptr->fsn->type != tok::Assign) {
            // TODO: GetType
            // TODO: Error
        } else if (ptr->fsn == ptr->lsn) {
            // TODO: Error
        } else if (ptr->fsn->ne != ptr->lsn) {
            // TODO: Error
        } else {
            calculate(ptr->fsn);
            NumberType result = calculate(ptr->lsn).GetValue();
            NumberType &variable = variables[ptr->fsn->GetIdentifier()];
            switch (ptr->type) {
            case tok::Assign: variable = result; break;
            case tok::PlusAssign: variable += result; break;
            case tok::MinusAssign: variable -= result; break;
            case tok::MulAssign: variable *= result; break;
            // TODO: /0
            case tok::DivAssign: variable /= result; break;
            }
            return Value(ptr->fsn->GetIdentifier(), variable);
        }
    }
    if (ptr->fsn && ptr->fsn == ptr->lsn) {
	    switch (ptr->type) {
        case tok::Plus:
            return calculate(ptr->fsn);
        case tok::Minus:
            return Value(calculate(ptr->fsn).GetValue().opposite());
        default: break;
        }
    }
	switch (ptr->type) {
	case tok::Nothing: return Value(NumberType());
	case tok::Number: return Value(ptr->GetNumber());
	case tok::Plus: return Value(calculate(ptr->fsn).GetValue() + calculate(ptr->lsn).GetValue());
	case tok::Minus: return Value(calculate(ptr->fsn).GetValue() - calculate(ptr->lsn).GetValue());
	case tok::Mul: return Value(calculate(ptr->fsn).GetValue() * calculate(ptr->lsn).GetValue());
	case tok::Div: return Value(calculate(ptr->fsn).GetValue() / calculate(ptr->lsn).GetValue());
	// TODO:
	case tok::Identifier:
	// TODO:
	case tok::Function:
		
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