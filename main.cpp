#include <cstdio>
#include <iostream>
#include <string>

#include "number.h"
#include "value.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"

#include "value.cpp"
#include "lexer.cpp"
#include "parser.cpp"
#include "calc.cpp"

#define Assert(msg, cond) \
if (!(cond)) { \
	std::cerr << msg << " : " << #cond << " is false!"; \
}
#define AssertCalc(expr, num) \
do { \
	std::string result = Calc::calc(#expr).to_string(); \
	Assert(#expr, result == #num); \
} while(false)

int main() 
{
	std::string s = "1-(-2)";
	//AssertCalc(1 + 2, 3);
	//AssertCalc(1 - 2, -1);
	//s = "10000e-2"; 
	//s = "111*222";
	while (true) {
		std::getline(std::cin, s);
		if (s == "@E" || s == "exit") break;
		std::cout << Calc::calc(s).to_string() << std::endl;
	}
	return 0;
}
