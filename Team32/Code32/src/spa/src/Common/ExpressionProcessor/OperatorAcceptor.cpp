#include "OperatorAcceptor.h"

#include <regex>

bool Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic(string op) {
	return op == "+" || op == "-" || op == "*" || op == "/" || op == "%";
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptRelationalStrict(string op) {
	return op == "==" || op == "!=" || op == ">" || op == ">=" || op == "<" || op == "<=";
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptRelational(string op) {
	return acceptRelationalStrict(op) || acceptArithmetic(op);
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptBinaryLogical(string op) {
	return op == "&&" || op == "||";
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptUnaryLogical(string op) {
	return op == "!";
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptLogicalStrict(string op) {
	return acceptBinaryLogical(op) || acceptUnaryLogical(op);
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptLogical(string op) {
	return acceptLogicalStrict(op) || acceptRelational(op);
}