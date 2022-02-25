#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "number.h"

namespace Calc
{
const std::string kSpecialPrefix = std::string("@");
enum class tok {
	Nothing,
	Number, 
	Plus, // +
	Minus, // -
	Mul, // *
	Div, // /
	Assign, // =
	PlusAssign, // +=
	MinusAssign, // -=
	MulAssign, // *=
	DivAssign, // /=
	
	LeftParen, // (
	RightParen, // )
	Comma, // ,
	Semi, // ;
	
	Identifier,
	Function,
	EndLine, // \n
	End,
};
bool IsAssignToken(tok type) {
	return type == tok::Assign
		|| type == tok::PlusAssign || type == tok::MinusAssign
		|| type == tok::MulAssign || type == tok::DivAssign;
}
const std::vector<std::pair<tok, std::string>> Keywords = {
	
};

std::string getTokenName(tok type) {
	static std::map<tok, std::string> TokenName;
	if (TokenName.empty()) {
		TokenName[tok::Nothing] = "Nothing";
		TokenName[tok::Number] = "Number";
		TokenName[tok::Plus] = "Plus+";
		TokenName[tok::Minus] = "Minus-";
		TokenName[tok::Mul] = "Mul*";
		TokenName[tok::Div] = "Div/";
		TokenName[tok::Assign] = "Assign=";
		TokenName[tok::PlusAssign] = "PlusAssign+=";
		TokenName[tok::MinusAssign] = "MinusAssign-=";
		TokenName[tok::MulAssign] = "MulAssign*=";
		TokenName[tok::DivAssign] = "DivAssign/=";
		
		TokenName[tok::LeftParen] = "LeftParen(";
		TokenName[tok::RightParen] = "RightParen)";
		TokenName[tok::Comma] = "Comma,";
		TokenName[tok::Semi] = "Semi;";
		
		TokenName[tok::Identifier] = "Identifier";
		TokenName[tok::Function] = "Function";
		TokenName[tok::EndLine] = "Endline\\n";
		TokenName[tok::End] = "End";
	}
	return TokenName[type];
}

struct ExprNode : public std::enable_shared_from_this<ExprNode> {
	//using ExprNodePtr = std::shared_ptr<ExprNode>; // ExprNode*;
	using ExprNodePtr = std::shared_ptr<ExprNode>;
	template <class... Args>
	static ExprNodePtr MakeExprNodePtr(Args&&... args)
	{ return std::make_shared<ExprNode>(std::forward<Args>(args)...); }
	// TODO
	NumberType number; std::string identifier;
	tok type;
	ExprNodePtr pa, ne; // parents, next_brother
	ExprNodePtr fsn, lsn; // first/last son
	explicit ExprNode(tok type_) : type(type_)
	{ pa = ne = fsn = lsn = nullptr; }
	explicit ExprNode(const NumberType &num) : type(tok::Number)
	{ pa = ne = fsn = lsn = nullptr; number = num; }
	explicit ExprNode(const std::string &str) : type(tok::Identifier)
	{ pa = ne = fsn = lsn = nullptr; identifier = str; }
	void SetChildren(ExprNodePtr lhs) {
		this->fsn = this->lsn = lhs;
		//lhs->pa = ExprNodePtr(this);
	}
	void SetChildren(ExprNodePtr lhs, ExprNodePtr rhs) {
		this->fsn = lhs, this->lsn = rhs;
		lhs->ne = rhs;
		//lhs->pa = rhs->pa = ExprNodePtr(this);
	}
	bool HasChildren() const { return fsn != nullptr; }
	// TODO:
	bool IsNothing() const { return type == tok::Nothing; }
	NumberType GetNumber() const { return number; }
	void SetNumber(const NumberType &number_) { number = number_; }
	std::string GetIdentifier() const { return identifier; }
	void SetIdentifier(const std::string &identifier_) { number = identifier_; }
};
using ExprNodePtr = ExprNode::ExprNodePtr;
template <class... Args>
ExprNodePtr MakeExprNodePtr(Args&&... args) {
	return ExprNode::MakeExprNodePtr(std::forward<Args>(args)...);
}
ExprNodePtr nothingNode(MakeExprNodePtr(tok::Nothing));
}
