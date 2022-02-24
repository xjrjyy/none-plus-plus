#pragma once

#include "token.h"
#include "lexer.h"

namespace Calc
{
class Parser {
public:
	static bool IsEndToken(EToken);
	void Move();
	bool Match(EToken type);
	bool ForceMatch(EToken type);
	Parser(Lexer* lexer_);
	
	ExprNodePtr NumberExpression();
	ExprNodePtr MultiplicativeExpression();
	ExprNodePtr AdditiveExpression();
	ExprNodePtr ArgumentExpressionList();
	ExprNodePtr Expression();
	ExprNodePtr AssignmentExpression();
	ExprNodePtr Parse();
private:
	Lexer *lexer;
	ExprNodePtr look;
};
}
