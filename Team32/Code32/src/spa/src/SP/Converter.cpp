#include "Converter.h"

ArithmeticOperator Converter::convertArithmetic(string op) {
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

RelationalOperator Converter::convertRelational(string op) {
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