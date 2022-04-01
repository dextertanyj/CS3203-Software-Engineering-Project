#include "Common/ExpressionProcessor/OperatorAcceptor.h"

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

bool Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic(const string& opr) {
	return opr == "+" || opr == "-" || opr == "*" || opr == "/" || opr == "%";
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptArithmetic(MathematicalOperator opr) {
	return opr == MathematicalOperator::Plus || opr == MathematicalOperator::Minus || opr == MathematicalOperator::Times ||
	       opr == MathematicalOperator::Divide || opr == MathematicalOperator::Modulo;
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptRelationalStrict(const string& opr) {
	return opr == "==" || opr == "!=" || opr == ">" || opr == ">=" || opr == "<" || opr == "<=";
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptRelationalStrict(MathematicalOperator opr) {
	return opr == MathematicalOperator::EQ || opr == MathematicalOperator::NEQ || opr == MathematicalOperator::GT ||
	       opr == MathematicalOperator::GTE || opr == MathematicalOperator::LT || opr == MathematicalOperator::LTE;
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptRelational(const string& opr) {
	return acceptRelationalStrict(opr) || acceptArithmetic(opr);
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptRelational(MathematicalOperator opr) {
	return acceptRelationalStrict(opr) || acceptArithmetic(opr);
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptBinaryLogical(const string& opr) { return opr == "&&" || opr == "||"; }

bool Common::ExpressionProcessor::OperatorAcceptor::acceptBinaryLogical(MathematicalOperator opr) {
	return opr == MathematicalOperator::And || opr == MathematicalOperator::Or;
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptUnaryLogical(const string& opr) { return opr == "!"; }

bool Common::ExpressionProcessor::OperatorAcceptor::acceptUnaryLogical(MathematicalOperator opr) {
	return opr == MathematicalOperator::Not;
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptLogicalStrict(const string& opr) {
	return acceptBinaryLogical(opr) || acceptUnaryLogical(opr);
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptLogicalStrict(MathematicalOperator opr) {
	return acceptBinaryLogical(opr) || acceptUnaryLogical(opr);
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptLogical(const string& opr) {
	return acceptLogicalStrict(opr) || acceptRelational(opr);
}

bool Common::ExpressionProcessor::OperatorAcceptor::acceptLogical(MathematicalOperator opr) {
	return acceptLogicalStrict(opr) || acceptRelational(opr);
}