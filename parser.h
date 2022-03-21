#pragma once

#include "token.h"
#include "lexer.h"

namespace Calc
{
class Parser {
public:
	static bool IsEndToken(tok);
	void Move();
	bool Match(tok type);
	bool ForceMatch(tok type);
	Parser(Lexer* lexer_);

	ExprNodePtr declaration();
	ExprNodePtr functionDefinition();
	ExprNodePtr expressionStatement();
	ExprNodePtr selectionStatement();
	ExprNodePtr iterationStatement();
	ExprNodePtr statement();
	
	ExprNodePtr unaryExpression();
	ExprNodePtr multiplicativeExpression();
	ExprNodePtr additiveExpression();
	ExprNodePtr argumentExpressionList();
	ExprNodePtr primaryExpression();
	ExprNodePtr assignmentExpression();
	ExprNodePtr expression();
	ExprNodePtr Parse();
private:
	friend class Lexer;
	Lexer *lexer;
	ExprNodePtr look;
};
}
