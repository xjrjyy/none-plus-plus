#include <cstdio>
#include <iostream>
#include <string>
#include <memory>
#include <map>
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

NumberType calc(ExprNodePtr ptr)
{
	// function
	if (ptr->type == tok::Identifier) {

		ArgsType args;
		for (ExprNodePtr p = ptr->fsn; p != nullptr && !p->IsNothing(); p = p->ne)
			args.push_back(calc(p));
		return GetFunction(ptr->GetIdentifier())(args);
	}
	switch (ptr->type) {
	case tok::Nothing: return NumberType();
	case tok::Number: return ptr->GetNumber();
	case tok::Plus: return calc(ptr->fsn) + calc(ptr->lsn);
	case tok::Minus: return calc(ptr->fsn) - calc(ptr->lsn);
	case tok::Mul: return calc(ptr->fsn) * calc(ptr->lsn);
	case tok::Div: return calc(ptr->fsn) / calc(ptr->lsn);
	
	// TODO
	case tok::Identifier:
	// TODO
	case tok::Function:
		
	// TODO: Error
	default: 
		return NumberType();
	}
}

NumberType calc(const std::string& expr)
{
	Lexer lexer(expr);
	Parser parser(&lexer);
	ExprNodePtr root = parser.Parse();
	return calc(root);
}
}