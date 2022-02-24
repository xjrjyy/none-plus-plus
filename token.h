#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "number.h"

namespace Calc
{
const std::string kSpecialPrefix = std::string("@");
enum class EToken {
	Nothing,
	Number, 
	Plus, // +
	Sub, // -
	Mul, // *
	Div, // /
	Equal, // =
	
	LeftParen, // (
	RightParen, // )
	Comma, // ,

	Var, // var
	
	Identifier,
	Function,
	End,
};
const std::vector<std::pair<EToken, std::string>> Keywords = {
	{EToken::Var, "var"},
};

std::string getTokenName(EToken type) {
	static std::map<EToken, std::string> TokenName;
	if (TokenName.empty()) {
		TokenName[EToken::Nothing] = "Nothing";
		TokenName[EToken::Number] = "Number";
		TokenName[EToken::Plus] = "Plus";
		TokenName[EToken::Sub] = "Sub";
		TokenName[EToken::Mul] = "Mul";
		TokenName[EToken::Div] = "Div";
		TokenName[EToken::Equal] = "Equal";
		
		TokenName[EToken::LeftParen] = "LeftParen";
		TokenName[EToken::RightParen] = "RightParen";
		TokenName[EToken::Comma] = "Comma";

		TokenName[EToken::Var] = "Var";
		
		TokenName[EToken::Identifier] = "Identifier";
		TokenName[EToken::Function] = "Function";
		TokenName[EToken::End] = "End";
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
	EToken type;
	ExprNodePtr pa, ne; // parents, next_brother
	ExprNodePtr fsn, lsn; // first/last son
	explicit ExprNode(EToken type_) : type(type_)
	{ pa = ne = fsn = lsn = nullptr; }
	explicit ExprNode(const NumberType &num) : type(EToken::Number)
	{ pa = ne = fsn = lsn = nullptr; number = num; }
	explicit ExprNode(const std::string &str) : type(EToken::Identifier)
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
	bool IsNothing() const { return type == EToken::Nothing; }
	NumberType GetNumber() const { return number; }
	void SetNumber(const NumberType& number_) { number = number_; }
	std::string GetIdentifier() const { return identifier; }
};
using ExprNodePtr = ExprNode::ExprNodePtr;
template <class... Args>
ExprNodePtr MakeExprNodePtr(Args&&... args) {
	return ExprNode::MakeExprNodePtr(std::forward<Args>(args)...);
}
ExprNodePtr nothingNode(new ExprNode(EToken::Nothing));
}
