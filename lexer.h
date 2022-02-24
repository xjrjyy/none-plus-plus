#pragma once

#include <string>

#include "token.h"

namespace Calc
{
class Lexer {
	std::string expr;
	std::size_t pos;
public:
	Lexer(const std::string &);
	bool CheckPos() const;
	bool CheckNextPos() const;
	char& c();
	char& nc();
	static bool SpaceChar(const char &);
	static bool NumberChar(const char &);
	static bool IdentifierChar(const char &);
	bool MatchString(const std::string &) const;
	
	ExprNodePtr next();
};
	
}
