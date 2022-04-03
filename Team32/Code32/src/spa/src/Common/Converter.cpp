#include "Common/Converter.h"

#include <string>
#include <unordered_map>

#include "Common/Validator.h"

MathematicalOperator Common::Converter::convertMathematical(const std::string& opr) {
	static const std::unordered_map<std::string, MathematicalOperator> map = {
		{"+", MathematicalOperator::Plus},   {"-", MathematicalOperator::Minus},  {"*", MathematicalOperator::Times},
		{"/", MathematicalOperator::Divide}, {"%", MathematicalOperator::Modulo}, {">", MathematicalOperator::GT},
		{">=", MathematicalOperator::GTE},   {"<", MathematicalOperator::LT},     {"<=", MathematicalOperator::LTE},
		{"==", MathematicalOperator::EQ},    {"!=", MathematicalOperator::NEQ},   {"!", MathematicalOperator::Not},
		{"&&", MathematicalOperator::And},   {"||", MathematicalOperator::Or}};
	auto iter = map.find(opr);
	if (iter == map.end()) {
		throw ConversionException("Expected mathematical symbol, got " + opr + " instead.");
	}
	return iter->second;
}

std::string Common::Converter::mathematicalToString(const MathematicalOperator& opr) {
	static const std::unordered_map<MathematicalOperator, std::string> map = {
		{MathematicalOperator::Plus, "+"},   {MathematicalOperator::Minus, "-"},  {MathematicalOperator::Times, "*"},
		{MathematicalOperator::Divide, "/"}, {MathematicalOperator::Modulo, "%"}, {MathematicalOperator::GT, ">"},
		{MathematicalOperator::GTE, ">="},   {MathematicalOperator::LT, "<"},     {MathematicalOperator::LTE, "<="},
		{MathematicalOperator::EQ, "=="},    {MathematicalOperator::NEQ, "!="},   {MathematicalOperator::Not, "!"},
		{MathematicalOperator::And, "&&"},   {MathematicalOperator::Or, "||"}};
	return map.at(opr);
}

ConstVal Common::Converter::convertInteger(const std::string& integer) {
	if (!Common::Validator::validateInteger(integer)) {
		throw ConversionException("Invalid integer provided: " + integer + ".");
	}
	try {
		return std::stoull(integer);
	} catch (std::invalid_argument&) {
		throw ConversionException("Invalid argument.");
	} catch (std::out_of_range&) {
		throw ConversionException("Integer out of range.");
	}
}
