#include "Common/ExpressionProcessor/UnaryLogicalNode.h"

#include <utility>

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

Common::ExpressionProcessor::UnaryLogicalNode::UnaryLogicalNode(MathematicalOperator opr, std::shared_ptr<LogicalNode> expression)
	: LogicalNode({std::move(expression), opr}) {
	if (!OperatorAcceptor::acceptUnaryLogical(opr)) {
		throw ExpressionProcessorException("Expected unary logical operator");
	}
}