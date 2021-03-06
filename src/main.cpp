#include <cstdio>
#include <iostream>
#include <string>

#include "number.h"
#include "value.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "calculator.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"

// #include "value.cpp"
// #include "lexer.cpp"
// #include "parser.cpp"
// #include "calculator.cpp"

#define Assert(msg, cond) \
if (!(cond)) { \
	std::cerr << msg << " : " << #cond << " is false!"; \
}
#define AssertCalc(expr, num) \
do { \
	std::string result = Calc::Calculator().calculate(#expr).GetValue().to_string(); \
	Assert(#expr, result == #num); \
} while(false)

Calc::Calculator calc;

int main(int argc, const char *argv[]) 
{
	llvm::InitLLVM X(argc, argv);
	std::string s, t;
	//AssertCalc(1 + 2, 3);
	//AssertCalc(1 - 2, -1);
	//s = "10000e-2"; 
	//s = "111*222";
	// s = "def a() {\n"
	// 	"    b += c\n"
	// 	"}";
	// std::cout << calc.calculate(s).GetValue().to_string() << std::endl;
	while (true) {
		std::getline(std::cin, s); s += "\n";
		llvm::outs() << calc.calculate(s).GetValue().to_string() << "\n";
		// std::cout << calc.calculate(s).GetValue().to_string() << std::endl;
		// if (s == "E" || s == "@E" || s == "exit") {
		// 	std::cout << calc.calculate(t).GetValue().to_string() << std::endl;
		// 	t = "";
		// } else {
		// 	calc.calculate(t);
		// 	t += s + "\n";
		// }
	}
	return 0;
}
