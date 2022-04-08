#include "Common/Converter.h"

#include <string>
#include <unordered_map>

#include "Common/Validator.h"

#define PLUS "+"
#define MINUS "-"
#define TIMES "*"
#define DIVIDE "/"
#define MODULO "%"
#define GREATER_THAN ">"
#define GREATER_THAN_EQUAL ">="
#define LESS_THAN "<"
#define LESS_THAN_EQUAL "<="
#define EQUAL "=="
#define NOT_EQUAL "!="
#define NOT "!"
#define AND "&&"
#define OR "||"

MathematicalOperator Common::Converter::convertMathematical(const std::string& opr) {
	static const std::unordered_map<std::string, MathematicalOperator> map = {{PLUS, MathematicalOperator::Plus},
	                                                                          {MINUS, MathematicalOperator::Minus},
	                                                                          {TIMES, MathematicalOperator::Times},
	                                                                          {DIVIDE, MathematicalOperator::Divide},
	                                                                          {MODULO, MathematicalOperator::Modulo},
	                                                                          {GREATER_THAN, MathematicalOperator::GT},
	                                                                          {GREATER_THAN_EQUAL, MathematicalOperator::GTE},
	                                                                          {LESS_THAN, MathematicalOperator::LT},
	                                                                          {LESS_THAN_EQUAL, MathematicalOperator::LTE},
	                                                                          {EQUAL, MathematicalOperator::EQ},
	                                                                          {NOT_EQUAL, MathematicalOperator::NEQ},
	                                                                          {NOT, MathematicalOperator::Not},
	                                                                          {AND, MathematicalOperator::And},
	                                                                          {OR, MathematicalOperator::Or}};
	auto iter = map.find(opr);
	if (iter == map.end()) {
		throw ConversionException("Expected mathematical symbol, got " + opr + " instead.");
	}
	return iter->second;
}

std::string Common::Converter::mathematicalToString(const MathematicalOperator& opr) {
	static const std::unordered_map<MathematicalOperator, std::string> map = {{MathematicalOperator::Plus, PLUS},
	                                                                          {MathematicalOperator::Minus, MINUS},
	                                                                          {MathematicalOperator::Times, TIMES},
	                                                                          {MathematicalOperator::Divide, DIVIDE},
	                                                                          {MathematicalOperator::Modulo, MODULO},
	                                                                          {MathematicalOperator::GT, GREATER_THAN},
	                                                                          {MathematicalOperator::GTE, GREATER_THAN_EQUAL},
	                                                                          {MathematicalOperator::LT, LESS_THAN},
	                                                                          {MathematicalOperator::LTE, LESS_THAN_EQUAL},
	                                                                          {MathematicalOperator::EQ, EQUAL},
	                                                                          {MathematicalOperator::NEQ, NOT_EQUAL},
	                                                                          {MathematicalOperator::Not, NOT},
	                                                                          {MathematicalOperator::And, AND},
	                                                                          {MathematicalOperator::Or, OR}};
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
