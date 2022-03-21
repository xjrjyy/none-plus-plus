#include "parser.h"

#include <iostream>


namespace Calc
{
bool Parser::IsEndToken(tok type) {
	return type == tok::Assign || type == tok::LeftParen
		|| type == tok::Comma;
}
void Parser::Move()  { 
	//tok lastType = look == nullptr ? tok::Nothing : look->type;
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
	// std::cerr << getTokenName(look->type) << " ";
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

// TODO: TLE
#define Enter() \
std::size_t curr_pos = lexer->pos; \
ExprNodePtr curr_look = MakeExprNodePtr(*look)

#define Leave() \
do { \
lexer->pos = curr_pos; \
look = curr_look; \
return nothingNode; \
} while (false)

#define TryToMatch(expr) \
do { \
ExprNodePtr tmp = expr; \
if (!tmp->IsNothing()) \
	return tmp; \
} while (false)

ExprNodePtr Parser::declaration() {
	Enter();
	// : functionDefinition
    // | statement
	TryToMatch(functionDefinition());
	TryToMatch(statement());
	Leave();
}

ExprNodePtr Parser::functionDefinition() {
	Enter();
	// : Definition Identifier 
    // '(' (Identifier (',' Identifier)*)? ')'
	// '{' statement* '}'
	if (!Match(tok::Kw_Def))
		return nothingNode;
	ExprNodePtr ptr = look; Move();
	if (!Match(tok::Identifier))
		ForceMatch(tok::Identifier);
	ptr->SetIdentifier(look->GetIdentifier()); Move();
	ForceMatch(tok::LeftParen);
	if (Match(tok::Identifier)) {
		// TODO: #1
	}
	ForceMatch(tok::RightParen);
	ForceMatch(tok::LeftBrace);
	while (!Match(tok::RightBrace)) {
		ExprNodePtr new_expr = statement();
		if (new_expr->IsNothing()) {
			// TODO: Error
			if (Match(tok::Nothing))
				Leave();
			else
			 	Move();
		}
		ptr->AppendChildren(new_expr);
	}
	ForceMatch(tok::RightBrace);
	return ptr;
}

ExprNodePtr Parser::expressionStatement() {
	// : expression? ';'
	Enter();
	ExprNodePtr ptr = expression();
	if (!Match(tok::Semi))
		Leave();
	Move();
	return ptr;
}

ExprNodePtr Parser::selectionStatement() {
	// : 'if' expression '{' statement '}'
	if (!Match(tok::Kw_If)) return nothingNode;
	ExprNodePtr expr = look; Move();
	expr->SetChildren(expression());
	ForceMatch(tok::LeftBrace);
	expr->AppendChildren(statement());
	ForceMatch(tok::RightBrace);
	return expr;
}

ExprNodePtr Parser::iterationStatement() {
	// : 'while' expression '{' statement '}'
	if (!Match(tok::Kw_While)) return nothingNode;
	ExprNodePtr expr = look; Move();
	expr->SetChildren(expression());
	ForceMatch(tok::LeftBrace);
	expr->AppendChildren(statement());
	ForceMatch(tok::RightBrace);
	return expr;
}

ExprNodePtr Parser::statement() {
	// : expressionStatement
	// | selectionStatement
	// | iterationStatement
	TryToMatch(expressionStatement());
	TryToMatch(selectionStatement());
	TryToMatch(iterationStatement());
	return nothingNode;
}

ExprNodePtr Parser::unaryExpression() {
	// : Identifier ('(' argumentExpressionList ')')?
	// | Number
	// | ('+'|'-') unaryExpression
	// | primaryExpression
	ExprNodePtr ptr;
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
	ptr = primaryExpression();
	if (!ptr->IsNothing())
		return ptr;
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
	// : assignmentExpression (',' assignmentExpression)* (',')?
	ExprNodePtr ptr = assignmentExpression();
	ExprNodePtr lst = ptr;
	if (ptr->IsNothing()) return nothingNode;
	while (Match(tok::Comma)) {
		Move();
		ExprNodePtr nxt = assignmentExpression();
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
	// : assignmentExpression (',' assignmentExpression)*
	ExprNodePtr ptr;
	ptr = assignmentExpression();
	if (Match(tok::Comma)) {
		ExprNodePtr expr = look, lst = ptr;
		expr->SetChildren(ptr);
		while (Match(tok::Comma)) {
			Move();
			expr->lsn = lst->ne = assignmentExpression();
			lst = lst->ne;
			// lst->pa = expr;
		}
		ptr = expr;
	}
	if (ptr->IsNothing()) return nothingNode;
	return ptr;
}
ExprNodePtr Parser::Parse() {
	// : statement
	return declaration();
}

}
