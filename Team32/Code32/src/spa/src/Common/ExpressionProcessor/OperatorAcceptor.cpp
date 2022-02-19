#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;

Acceptor Common::ExpressionProcessor::OperatorAcceptor::getAcceptor(ExpressionType type) {
	switch (type) {
		case ExpressionType::Arithmetic:
			return Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic;
		case ExpressionType::Relational:
			return Common::ExpressionProcessor::OperatorAcceptor::acceptRelational;
		case ExpressionType::Logical:
			return Common::ExpressionProcessor::OperatorAcceptor::acceptLogical;
		default:
			throw ExpressionProcessorException("Unknown expression type received.");
	}
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic(const string& op) {
	return op == "+" || op == "-" || op == "*" || op == "/" || op == "%";
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic(MathematicalOperator op) {
	return op == MathematicalOperator::Plus || op == MathematicalOperator::Minus || op == MathematicalOperator::Times ||
	       op == MathematicalOperator::Divide || op == MathematicalOperator::Modulo;
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptRelationalStrict(const string& op) {
	return op == "==" || op == "!=" || op == ">" || op == ">=" || op == "<" || op == "<=";
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptRelationalStrict(MathematicalOperator op) {
	return op == MathematicalOperator::EQ || op == MathematicalOperator::NEQ || op == MathematicalOperator::GT ||
	       op == MathematicalOperator::GTE || op == MathematicalOperator::LT || op == MathematicalOperator::LTE;
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptRelational(const string& op) {
	return acceptRelationalStrict(op) || acceptArithmetic(op);
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptRelational(MathematicalOperator op) {
	return acceptRelationalStrict(op) || acceptArithmetic(op);
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptBinaryLogical(const string& op) { return op == "&&" || op == "||"; }

bool Common::ExpressionProcessor::OperatorAcceptor::acceptBinaryLogical(MathematicalOperator op) {
	return op == MathematicalOperator::And || op == MathematicalOperator::Or;
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptUnaryLogical(const string& op) { return op == "!"; }

bool Common::ExpressionProcessor::OperatorAcceptor::acceptUnaryLogical(MathematicalOperator op) { return op == MathematicalOperator::Not; }

bool Common::ExpressionProcessor::OperatorAcceptor::acceptLogicalStrict(const string& op) {
	return acceptBinaryLogical(op) || acceptUnaryLogical(op);
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptLogicalStrict(MathematicalOperator op) {
	return acceptBinaryLogical(op) || acceptUnaryLogical(op);
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptLogical(const string& op) {
	return acceptLogicalStrict(op) || acceptRelational(op);
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptLogical(MathematicalOperator op) {
	return acceptLogicalStrict(op) || acceptRelational(op);
}