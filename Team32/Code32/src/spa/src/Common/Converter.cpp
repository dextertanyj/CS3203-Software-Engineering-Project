#include "Converter.h"

#include <string>

#include "Common/Validator.h"

using namespace std;

MathematicalOperator Common::Converter::convertMathematical(const string& op) {
	if (op == "+") {
		return MathematicalOperator::Plus;
	}
	if (op == "-") {
		return MathematicalOperator::Minus;
	}
	if (op == "*") {
		return MathematicalOperator::Times;
	}
	if (op == "/") {
		return MathematicalOperator::Divide;
	}
	if (op == "%") {
		return MathematicalOperator::Modulo;
	}
	if (op == ">") {
		return MathematicalOperator::GT;
	}
	if (op == ">=") {
		return MathematicalOperator::GTE;
	}
	if (op == "<") {
		return MathematicalOperator::LT;
	}
	if (op == "<=") {
		return MathematicalOperator::LTE;
	}
	if (op == "==") {
		return MathematicalOperator::EQ;
	}
	if (op == "!=") {
		return MathematicalOperator::NEQ;
	}
	if (op == "!") {
		return MathematicalOperator::Not;
	}
	if (op == "&&") {
		return MathematicalOperator::And;
	}
	if (op == "||") {
		return MathematicalOperator::Or;
	}
	throw ConversionException("Expected mathematical symbol, got " + op + " instead.");
}

int Common::Converter::convertInteger(const string& integer) {
	if (!Common::Validator::validateInteger(integer)) {
		throw ConversionException("Invalid argument");
	}
	try {
		return stoi(integer);
	} catch (invalid_argument&) {
		throw ConversionException("Invalid argument");
	} catch (out_of_range&) {
		throw ConversionException("Integer out of range");
	}
}
