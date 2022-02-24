#include "parser.h"

#include <iostream>


namespace Calc
{
bool Parser::IsEndToken(EToken type) {
	return type == EToken::Equal || type == EToken::LeftParen
		|| type == EToken::Comma;
}
void Parser::Move()  { 
	EToken lastType = EToken::Nothing;
	if (look != nullptr)
		lastType = look->type;
	look = lexer->next(); 
	if (look->type == EToken::Plus || look->type == EToken::Sub) {
		//std::cout << getTokenName(lastType) << std::endl;
		//if (lastType != EToken::Number) {
		// TODO: ;
		if (IsEndToken(lastType)) {
			bool isMinus = look->type == EToken::Sub;
			look = lexer->next();
			if (look->type != EToken::Number) {
				// TODO: Error
				return;
			}
			if (isMinus)
				look->SetNumber(look->GetNumber().opposite());
		}
	}
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

ExprNodePtr Parser::NumberExpression() {
	// : '(' primaryExpression ')'
	// | Identifier ('(' argumentExpressionList ')')?
	// | Number
	ExprNodePtr ptr;
	if (Match(EToken::LeftParen)) {
		Move(); ptr = PrimaryExpression();
		ForceMatch(EToken::RightParen);
		return ptr;
	}
	if (Match(EToken::Identifier)) {
		ptr = look; Move();
		if (Match(EToken::LeftParen)) {
			Move(); 
			ExprNodePtr args = ArgumentExpressionList();
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
	return nothingNode;
}
ExprNodePtr Parser::MultiplicativeExpression() {
	// : numberExpression (('*'|'/') numberExpression)*
	ExprNodePtr ptr = NumberExpression();
	if (ptr->IsNothing()) return nothingNode;
	while (Match(EToken::Mul) || Match(EToken::Div)) {
		ExprNodePtr node = look; Move();
		ExprNodePtr rhs = NumberExpression();
		if (rhs->IsNothing()) {
			// TODO: Error
			return ptr;
		}
		node->SetChildren(ptr, rhs);
		ptr = node;
	}
	return ptr;
}

ExprNodePtr Parser::AdditiveExpression() {
	// : multiplicativeExpression (('+'|'-') multiplicativeExpression)*
	ExprNodePtr ptr = MultiplicativeExpression();
	if (ptr->IsNothing()) return nothingNode;
	while (Match(EToken::Plus) || Match(EToken::Sub)) {
		ExprNodePtr node = look; Move();
		ExprNodePtr rhs = MultiplicativeExpression();
		if (rhs->IsNothing()) {
			// TODO: Error
			return ptr;
		}
		node->SetChildren(ptr, rhs);
		ptr = node;
	}
	return ptr;
}
ExprNodePtr Parser::ArgumentExpressionList() {
	// : primaryExpression (',' primaryExpression)* (',')?
	ExprNodePtr ptr = PrimaryExpression();
	ExprNodePtr lst = ptr;
	if (ptr->IsNothing()) return nothingNode;
	while (Match(EToken::Comma)) {
		Move();
		ExprNodePtr nxt = PrimaryExpression();
		// (',')?
		if (nxt->IsNothing()) return ptr;
		lst->ne = nxt; lst = nxt;
	}
	return ptr;
}
ExprNodePtr Parser::AssignmentExpression() {
	// : additiveExpression ('=' primaryExpression)?
	ExprNodePtr ptr = AdditiveExpression();
	if (Match(EToken::Equal)) {
		ExprNodePtr assignment = look;
		Move();
		assignment->SetChildren(ptr, PrimaryExpression());
		assignment->SetIdentifier(ptr->GetIdentifier());
		ptr = assignment;
	}
	return ptr;
}
ExprNodePtr Parser::PrimaryExpression() {
	// : assignmentExpression
	ExprNodePtr ptr;
	ptr = AssignmentExpression();
	if (ptr->IsNothing()) return nothingNode;
	return ptr;
}
ExprNodePtr Parser::Parse() {
	return PrimaryExpression();
}

}
