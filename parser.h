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
	
	ExprNodePtr unaryExpression();
	ExprNodePtr multiplicativeExpression();
	ExprNodePtr additiveExpression();
	ExprNodePtr argumentExpressionList();
	ExprNodePtr primaryExpression();
	ExprNodePtr assignmentExpression();
	ExprNodePtr expression();
	ExprNodePtr declarator();
	ExprNodePtr declaration();
	ExprNodePtr Parse();
private:
	Lexer *lexer;
	ExprNodePtr look;
};
}
