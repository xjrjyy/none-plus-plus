#include "calculator.h"

#include <cstddef>
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
	if (ptr->type == tok::Comma || ptr->type == tok::Semi) {
        for (ExprNodePtr expr = ptr->fsn; expr != nullptr; expr = expr->ne) {
            if (expr == ptr->lsn)
                return calculate(expr);
            else
                calculate(expr);
        }
        return Value(NumberType());
	}
	if (ptr->type == tok::Identifier) {
        std::string identifier = ptr->GetIdentifier();
        if (ptr->HasChildren()) { // function
            // TODO: Error
        }
        if (constants.count(identifier)) return Value(identifier, constants[identifier]);
        return Value(identifier, variables[identifier]);
	}
    if (ptr->type == tok::Function) {
        std::string identifier = ptr->GetIdentifier();
        ArgsType args;
        for (ExprNodePtr p = ptr->fsn; p != nullptr; p = p->ne)
            args.push_back(calculate(p).GetValue());
        std::pair<bool, FunctionType> func = GetFunction(identifier);
        if (func.first) {
            return Value(GetResultName(identifier), func.second(args));
        } else if (functions.count(identifier)) {
            auto variables_bak = variables;
            ExprNodePtr func = functions[identifier];
            ExprNodePtr p;
            std::size_t pos = 0;
            for (p = func->fsn; p->type != tok::Semi; p = p->ne, ++pos) {
                if (pos >= args.size()) {
                    // TODO: Error
                    continue;
                }
                variables[p->GetIdentifier()] = args[pos];
            }
            if (p->ne == nullptr) {
                return Value(GetResultName(identifier), NumberType());
            }
            for (p = p->ne; p != func->lsn; p = p->ne) {
                calculate(p);
            }
            NumberType result = calculate(func->lsn).GetValue();
            variables = variables_bak;
            return Value(GetResultName(identifier), result);
        } else {
            return Value(GetResultName(identifier), NumberType());
        }
    }
	if (ptr->type == tok::Kw_Def) {
        std::string identifier = ptr->GetIdentifier();
        functions[identifier] = ptr;
        return Value("def", NumberType());
	}
	if (ptr->type == tok::Kw_If) {
        // TODO: else
        Value cond = calculate(ptr->fsn);
        if (cond.GetValue().to_bool()) {
            return Value(ptr->lsn->GetIdentifier(), calculate(ptr->lsn).GetValue());
        } else {
            return Value("if", NumberType());
        }
	}
	if (ptr->type == tok::Kw_While) {
        Value result = Value("while", NumberType());
        // TODO: while counter
        while (true) {
            Value cond = calculate(ptr->fsn);
            if (!cond.GetValue().to_bool()) break;
            result = Value(ptr->lsn->GetIdentifier(), calculate(ptr->lsn).GetValue());
        }
        return result;
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