#pragma once

#include <string>

#include "token.h"

namespace Calc
{
class Parser;
class Lexer {
public:
	Lexer(const std::string &);
	bool CheckPos() const;
	bool CheckNextPos() const;
	char& c();
	char& nc();
	void Move(int = 1);
	static bool SpaceChar(const char &);
	static bool NumberChar(const char &);
	static bool IdentifierChar(const char &);
	bool MatchString(const std::string &) const;
	
	ExprNodePtr next();
private:
	friend class Parser;
	std::string expr;
	std::size_t pos;
};
	
}
