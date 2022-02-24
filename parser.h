#pragma once

#include "token.h"
#include "lexer.h"

namespace Calc
{
class Parser {
public:
	void Move();
	bool Match(EToken type);
	bool ForceMatch(EToken type);
	Parser(Lexer* lexer_);
	
	ExprNodePtr NumberExpression();
	ExprNodePtr MultiplicativeExpression();
	ExprNodePtr AdditiveExpression();
	ExprNodePtr ArgumentExpressionList();
	ExprNodePtr Expression();
	ExprNodePtr DefineExpression();
	ExprNodePtr Parse();
private:
	Lexer* lexer;
	ExprNodePtr look;
};
}
