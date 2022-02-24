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

class Number {
public:
	using number_type = double;
	Number(int num = 0) {
		number = num;
	}
	Number(double num) {
		number = num;
	}
	Number(const std::string& s) {
		sscanf(s.c_str(), "%lf", &number);
	}
	virtual ~Number() = default;
	Number opposite() const {
		return -number;
	}
	Number operator+(const Number& rhs) const {
		return Number(number + rhs.number);
	}
	Number operator-(const Number& rhs) const {
		return Number(number - rhs.number);
	}
	Number operator*(const Number& rhs) const {
		return Number(number * rhs.number);
	}
	Number operator/(const Number& rhs) const {
		return Number(number / rhs.number);
	}
	// TODO
	static Number eNumber(const Number& base, const Number& exp) {
		return base * Number(pow(10, exp));
	}
	operator double() const {
		return number;
	}
	std::string to_string() const {
		return std::to_string(number);
	}
private:
	number_type number;
};

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
	std::getline(std::cin, s);
	//s = "111*222";
	std::cout << Calc::calc(s).to_string();
	return 0;
}
