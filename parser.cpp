#include "parser.h"

#include <iostream>


namespace Calc
{
bool Parser::IsEndToken(EToken type) {
	return type == EToken::Assign || type == EToken::LeftParen
		|| type == EToken::Comma;
}
void Parser::Move()  { 
	EToken lastType = EToken::Nothing;
	if (look != nullptr)
		lastType = look->type;
	look = lexer->next(); 
	/*
	if (look->type == EToken::Plus || look->type == EToken::Minus) {
		//std::cout << getTokenName(lastType) << std::endl;
		//if (lastType != EToken::Number) {
		// TODO: ;
		if (IsEndToken(lastType)) {
			bool isMinus = look->type == EToken::Minus;
			look = lexer->next();
			if (look->type != EToken::Number) {
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
bool Parser::Match(EToken type) {
	return look->type == type;
}
bool Parser::ForceMatch(EToken type) {
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

ExprNodePtr Parser::unaryExpression() {
	// : '(' primaryExpression ')'
	// | Identifier ('(' argumentExpressionList ')')?
	// | Number
	// | ('+'|'-') unaryExpression
	ExprNodePtr ptr;
	if (Match(EToken::LeftParen)) {
		Move(); ptr = primaryExpression();
		ForceMatch(EToken::RightParen);
		return ptr;
	}
	if (Match(EToken::Identifier)) {
		ptr = look; Move();
		if (Match(EToken::LeftParen)) {
			Move(); 
			ExprNodePtr args = argumentExpressionList();
			ForceMatch(EToken::RightParen);
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
	if (Match(EToken::Number)) {
		ptr = look; Move();
		return ptr;
	}
	if (Match(EToken::Plus) || Match(EToken::Minus)) {
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
	while (Match(EToken::Mul) || Match(EToken::Div)) {
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
	while (Match(EToken::Plus) || Match(EToken::Minus)) {
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
	while (Match(EToken::Comma)) {
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
		assignment->SetChildren(ptr, primaryExpression());
		assignment->SetIdentifier(ptr->GetIdentifier());
		ptr = assignment;
	}
	return ptr;
}
ExprNodePtr Parser::primaryExpression() {
	// : assignmentExpression
	ExprNodePtr ptr;
	ptr = assignmentExpression();
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
ExprNodePtr Parser::declarator() {
	// TODO:
}
ExprNodePtr Parser::declaration() {
	// TODO:
}
ExprNodePtr Parser::Parse() {
	return primaryExpression();
}

}
