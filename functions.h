#pragma once

#include <cstdio>
#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <limits>
#include <cstdlib>
#include <functional>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <utility>
#include <type_traits>

#ifndef M_E
#define M_E        2.71828182845904523536   // e
#endif
#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif

#include "number.h"
#include "value.h"
#include "token.h"
#include "lexer.h"

namespace Calc
{

// TODO:
// https://blog.csdn.net/qq_31175231/article/details/77479692
template<typename T>
struct has_member_foo
{
private:
    template<typename U>
        static auto Check(int) -> decltype( std::declval<U>().foo(), std::true_type() );
    template<typename U>
        static std::false_type Check(...);
public:
    enum { value = std::is_same<decltype(Check<T>(0)),std::true_type>::value  };
};

using ArgsType = std::vector<NumberType>;
using FunctionType = std::function<NumberType(const ArgsType&)>;
FunctionType emptyFunction = [](const ArgsType&) -> NumberType {
	return NumberType();
};

template <typename T1>
bool CheckArgsLength(const ArgsType& args, T1 len)
{ return args.size() == (std::size_t)len; }
template <typename T1, typename ...T>
bool CheckArgsLength(const ArgsType& args, T1 len, T... a) 
{ return CheckArgsLength(args, len) || CheckArgsLength(args, a...); }
template <typename ...T>
void CheckArgs(const ArgsType& args, T... a) {
	if (CheckArgsLength(args, a...)) return;
	// TODO: Error
	std::cerr << "Error: The number of arguments is wrong\n";
}

NumberType getValue(const ArgsType& args, std::size_t pos, const NumberType& def) 
{ return pos < args.size() ? args[pos] : def; }

#define DefineConstant(name, val) \
functions[#name] = [](const ArgsType& args) -> NumberType { \
	CheckArgs(args, 0); \
	return NumberType(val); \
}

#define DefineNumberFunction(name, def0) \
functions[#name] = [](const ArgsType& args) -> NumberType { \
	CheckArgs(args, 1); \
	NumberType x = getValue(args, 0, def0); \
	return x.name(); \
}

#define DefineFunction_1(name, def0) \
functions[#name] = [](const ArgsType& args) -> NumberType { \
	CheckArgs(args, 1); \
	NumberType x = getValue(args, 0, def0); \
	return NumberType(name(x)); \
}

#define DefineRenameFunction_1(name, newname,  def0) \
functions[#newname] = [](const ArgsType& args) -> NumberType { \
	CheckArgs(args, 1); \
	NumberType x = getValue(args, 0, def0); \
	return NumberType(name(x)); \
}

#define DefineFunction_2(name, def0, def1) \
functions[#name] = [](const ArgsType& args) -> NumberType { \
	CheckArgs(args, 2); \
	NumberType x = getValue(args, 0, def0); \
	NumberType y = getValue(args, 1, def1); \
	return NumberType(name(x, y)); \
}

void InitFunctions(std::map<std::string, FunctionType>& functions) {
	DefineConstant(e, M_E);
	DefineConstant(pi, M_PI);
	
	//DefineNumberFunction(floor, 0);
	
	DefineFunction_1(sin, 0);
	DefineFunction_1(cos, 0);
	DefineFunction_1(tan, 0);
	
	DefineRenameFunction_1(log, ln, 1);
	DefineFunction_1(log2, 1);
	DefineFunction_1(log10, 1);
	DefineFunction_1(sqrt, 0);
	DefineFunction_2(pow, 0, 0);
}
FunctionType GetFunction(const std::string &name) 
{
	static std::map<std::string, FunctionType> functions;
	if (functions.empty()) InitFunctions(functions);
	if (functions.count(name)) return functions[name];
	return emptyFunction;
}
}
