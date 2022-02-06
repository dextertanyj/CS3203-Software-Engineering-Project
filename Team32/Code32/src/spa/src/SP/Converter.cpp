#include <string>

#include "Converter.h"
#include "Validator.h"

using namespace std;

ArithmeticOperator Converter::convertArithmetic(const string& op) {
	if (op == "+") {
		return ArithmeticOperator::Plus;
	}
	if (op == "-") {
		return ArithmeticOperator::Minus;
	}
	if (op == "*") {
		return ArithmeticOperator::Times;
	}
	if (op == "/") {
		return ArithmeticOperator::Divide;
	}
	if (op == "%") {
		return ArithmeticOperator::Modulo;
	}
	throw ConversionException("Expected arithmetic symbol, got " + op + " instead.");
}

RelationalOperator Converter::convertRelational(const string& op) {
	if (op == ">") {
		return RelationalOperator::GT;
	}
	if (op == ">=") {
		return RelationalOperator::GTE;
	}
	if (op == "<") {
		return RelationalOperator::LT;
	}
	if (op == "<=") {
		return RelationalOperator::LTE;
	}
	if (op == "==") {
		return RelationalOperator::EQ;
	}
	if (op == "!=") {
		return RelationalOperator::NEQ;
	}
	throw ConversionException("Expected relational symbol, got " + op + " instead.");
}

int Converter::convertInteger(const string& integer) {
	if (!Validator::validateInteger(integer)) {
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