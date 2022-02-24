#include "lexer.h"
#include "token.h"

#include <string>
#include <iostream>

namespace Calc
{
Lexer::Lexer(const std::string &s) : expr(s) { pos = 0; }
bool Lexer::CheckPos() const { return pos < expr.length(); }
bool Lexer::CheckNextPos() const { return pos + 1 < expr.length(); }
char& Lexer::c() { return expr[pos]; }
char& Lexer::nc() { return expr[pos + 1]; }
void Lexer::Move(int dis) { pos += dis; }
bool Lexer::SpaceChar(const char &ch)
{ return isspace(ch); }
bool Lexer::NumberChar(const char &ch)
{ return isdigit(ch) || ch == '.'; }
bool Lexer::IdentifierChar(const char &ch)
{ return isalpha(ch) || ch == '_'; }

bool Lexer::MatchString(const std::string &str) const {
	if (pos + str.size() > expr.size())
		return false;
	for (size_t p = pos; p < pos + str.size(); ++p)
		if (expr[p] != str[p - pos]) return false;
	return true;
}

ExprNodePtr Lexer::next() {
	while (CheckPos() && SpaceChar(c()))
		++pos;
	
	if (!CheckPos()) 
		return MakeExprNodePtr(EToken::Nothing);
	//	return MakeExprNodePtr(EToken::End);
	// TODO: 1e4 0xff +121 -121
	if (CheckNextPos() && c() == '0') {
		std::size_t lastPos = pos;
		NumberType number(0);
		if (nc() == 'b') {
			Move(2);
			while (CheckPos() && (c() >= '0' && c() <= '1')) {
				number = number * NumberType(2) + NumberType(c() - '0');
				Move();
			}
			if (pos == lastPos + 2) {
				// TODO: Error
			}
			return MakeExprNodePtr(number);
		}
		if (nc() == 'x') {
			Move(2);
			while (CheckPos() && (isdigit(c()) || (tolower(c()) >= 'a' && tolower(c()) <= 'f'))) {
				if (isdigit(c())) number = number * NumberType(16) + NumberType(c() - '0');
				else number = number * NumberType(16) + NumberType(tolower(c()) - 'a' + 10);
				Move();
			}
			if (pos == lastPos + 2) {
				// TODO: Error
			}
			return MakeExprNodePtr(number);
		}
		if (isdigit(nc())) {
			Move(2);
			while (CheckPos() && isdigit(c())) {
				if (c() >= '8') {
					// TODO: Error
				}
				number = number * NumberType(8) + NumberType(c() - '0');
				Move();
			}
			if (pos == lastPos + 2) {
				// TODO: Error
			}
			return MakeExprNodePtr(number);
		}
		/*
		while (checkPos() && numberChar(c())) 
			++pos;
		number = NumberType(expr.substr(lastPos, pos - lastPos));
		return MakeExprNodePtr(number);
		*/
	}
	/* (([1-9][0-9]*) | ('0'))
	 * ('.' [0-9]*)?
	 * ([eE] [+-]? (([1-9][0-9]*) | ('0')) )?
	 */
	if (isdigit(c())) {
		std::size_t lastPos = pos, intPos;
		std::size_t decPos = -1, ePos = -1;
		// [lastPos, intPos)
		if (c() > '0') {
			while (CheckPos() && isdigit(c())) 
				++pos;
		} else {
			++pos;
		}
		intPos = pos;
		// [intPos + 1, decPos)
		if (CheckPos() && c() == '.') {
			Move();
			while (CheckPos() && isdigit(c()))
				Move();
			decPos = pos;
		}
		NumberType number;
		// TODO
		if (decPos == (std::size_t)-1)
			decPos = intPos;
		number = NumberType(expr.substr(lastPos, decPos - lastPos));
		if (CheckPos() && tolower(c()) == 'e') {
			++pos;
			bool isMinus = false;
			if (CheckPos() && (c() == '+' || c() == '-'))
				isMinus = c() == '-', ++pos;
			if (!CheckPos() || !isdigit(c())) {
				// TODO: Error
				return MakeExprNodePtr(number);
			}
			if (c() == '0') {
				if (CheckNextPos() && isdigit(nc())) {
					// TODO: Error
				}
				return MakeExprNodePtr(number);
			}
			while (CheckPos() && isdigit(c()))
				++pos;
			ePos = pos;
			// TODO : exp
			NumberType exp(expr.substr(decPos + 1, ePos - (decPos + 1)));
			number = NumberType::eNumber(number, exp);
		}
		return MakeExprNodePtr(number);
	}
	if (IdentifierChar(c())) {
		for (auto p : Keywords) {
			if (MatchString(p.second)) {
				pos += p.second.size();
				return MakeExprNodePtr(p.first);
			}
		}

		std::size_t lastPos = pos;
		while (CheckPos() && (IdentifierChar(c()) || isdigit(c()))) 
			++pos;
		std::string identifier = expr.substr(lastPos, pos - lastPos);
		return MakeExprNodePtr(identifier);
	}

	ExprNodePtr ret = nullptr;
	switch (c()) {
	case '+': ret = MakeExprNodePtr(EToken::Plus); break;
	case '-': ret = MakeExprNodePtr(EToken::Sub); break;
	case '*': ret = MakeExprNodePtr(EToken::Mul); break;
	// TODO: 1 // 2
	case '/': ret = MakeExprNodePtr(EToken::Div); break;
	// TODO: 1 == 2 / 2
	case '=': ret = MakeExprNodePtr(EToken::Equal); break;
	case '(': ret = MakeExprNodePtr(EToken::LeftParen); break;
	case ')': ret = MakeExprNodePtr(EToken::RightParen); break;
	case ',': ret = MakeExprNodePtr(EToken::Comma); break;
	}
	++pos;
	if (ret == nullptr) ret = MakeExprNodePtr(EToken::Nothing);
	return ret;
}
	
}
