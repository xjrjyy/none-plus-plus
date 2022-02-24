#include "parser.h"

#include <iostream>


namespace Calc
{
bool Parser::IsEndToken(tok type) {
	return type == tok::Assign || type == tok::LeftParen
		|| type == tok::Comma;
}
void Parser::Move()  { 
	tok lastType = tok::Nothing;
	if (look != nullptr)
		lastType = look->type;
	look = lexer->next(); 
	/*
	if (look->type == tok::Plus || look->type == tok::Minus) {
		//std::cout << getTokenName(lastType) << std::endl;
		//if (lastType != tok::Number) {
		// TODO: ;
		if (IsEndToken(lastType)) {
			bool isMinus = look->type == tok::Minus;
			look = lexer->next();
			if (look->type != tok::Number) {
				// TODO: Error
				return;
			}
			if (isMinus)
				look->SetNumber(look->GetNumber().opposite());
		}
	}
	*/
	//std::cout << getTokenName(look->type) << std::endl;
}
bool Parser::Match(tok type) {
	return look->type == type;
}
bool Parser::ForceMatch(tok type) {
	if (!Match(type)) {
		// TODO: Error
		std::cerr << "Error: Could not match token " << getTokenName(type) 
			<< " (Matched token " << getTokenName(look->type) << " )\n";
		exit(1);
		return false;
	}
	Move();
	return true;
}
Parser::Parser(Lexer* lexer_) :  lexer(lexer_), look(nullptr)
{ Move(); }

ExprNodePtr Parser::expressionStatement() {
	// : expression? ';'
	ExprNodePtr ptr = expression();
	ForceMatch(tok::Semi);
	return ptr;
}
ExprNodePtr Parser::statement() {
	// : expressionStatement
	ExprNodePtr ptr;
	ptr = expressionStatement();
	if (!ptr->IsNothing())
		return ptr;
	return ptr;
}

ExprNodePtr Parser::unaryExpression() {
	// : '(' primaryExpression ')'
	// | Identifier ('(' argumentExpressionList ')')?
	// | Number
	// | ('+'|'-') unaryExpression
	ExprNodePtr ptr;
	if (Match(tok::LeftParen)) {
		Move(); ptr = primaryExpression();
		ForceMatch(tok::RightParen);
		return ptr;
	}
	if (Match(tok::Identifier)) {
		ptr = look; Move();
		if (Match(tok::LeftParen)) {
			Move(); 
			ExprNodePtr args = argumentExpressionList();
			ForceMatch(tok::RightParen);
			ExprNodePtr p = args;
			ptr->fsn = p;
			while (p != nullptr && p->ne != nullptr) {
				//p->pa = ptr;
				p = p->ne;
			}
			//if (p != nullptr) p->pa = ptr;
			ptr->lsn = p;
		}
		return ptr;
	}
	if (Match(tok::Number)) {
		ptr = look; Move();
		return ptr;
	}
	if (Match(tok::Plus) || Match(tok::Minus)) {
		ptr = look; Move();
		ptr->SetChildren(unaryExpression());
		return ptr;
	}
	return nothingNode;
}
ExprNodePtr Parser::multiplicativeExpression() {
	// : unaryExpression (('*'|'/') unaryExpression)*
	ExprNodePtr ptr = unaryExpression();
	if (ptr->IsNothing()) return nothingNode;
	while (Match(tok::Mul) || Match(tok::Div)) {
		ExprNodePtr node = look; Move();
		ExprNodePtr rhs = unaryExpression();
		if (rhs->IsNothing()) {
			// TODO: Error
			return ptr;
		}
		node->SetChildren(ptr, rhs);
		ptr = node;
	}
	return ptr;
}

ExprNodePtr Parser::additiveExpression() {
	// : multiplicativeExpression (('+'|'-') multiplicativeExpression)*
	ExprNodePtr ptr = multiplicativeExpression();
	if (ptr->IsNothing()) return nothingNode;
	while (Match(tok::Plus) || Match(tok::Minus)) {
		ExprNodePtr node = look; Move();
		ExprNodePtr rhs = multiplicativeExpression();
		if (rhs->IsNothing()) {
			// TODO: Error
			return ptr;
		}
		node->SetChildren(ptr, rhs);
		ptr = node;
	}
	return ptr;
}
ExprNodePtr Parser::argumentExpressionList() {
	// : primaryExpression (',' primaryExpression)* (',')?
	ExprNodePtr ptr = primaryExpression();
	ExprNodePtr lst = ptr;
	if (ptr->IsNothing()) return nothingNode;
	while (Match(tok::Comma)) {
		Move();
		ExprNodePtr nxt = primaryExpression();
		// (',')?
		if (nxt->IsNothing()) return ptr;
		lst->ne = nxt; lst = nxt;
	}
	return ptr;
}
ExprNodePtr Parser::assignmentExpression() {
	// : additiveExpression (('='|'+='|'-='|'*='|'/=') primaryExpression)?
	ExprNodePtr ptr = additiveExpression();
	if (IsAssignToken(look->type)) {
		ExprNodePtr assignment = look;
		Move();
		assignment->SetChildren(ptr, assignmentExpression());
		assignment->SetIdentifier(ptr->GetIdentifier());
		ptr = assignment;
	}
	return ptr;
}
ExprNodePtr Parser::primaryExpression() {
	// : '(' expression ')'
	ExprNodePtr ptr = nothingNode;
	if (Match(tok::LeftParen)) {
		Move();
		ptr = expression();
		ForceMatch(tok::RightParen);
	}
	if (ptr->IsNothing()) return nothingNode;
	return ptr;
}
ExprNodePtr Parser::expression() {
	// : assignmentExpression
	ExprNodePtr ptr;
	ptr = assignmentExpression();
	if (ptr->IsNothing()) return nothingNode;
	return ptr;
}
ExprNodePtr Parser::Parse() {
	// : statement
	return statement();
}

}
