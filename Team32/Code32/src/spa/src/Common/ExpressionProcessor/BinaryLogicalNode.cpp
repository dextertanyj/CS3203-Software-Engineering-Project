#include "Common/ExpressionProcessor/BinaryLogicalNode.h"

#include <utility>

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

Common::ExpressionProcessor::BinaryLogicalNode::BinaryLogicalNode(MathematicalOperator opr, shared_ptr<LogicalNode> lhs,
                                                                  shared_ptr<LogicalNode> rhs)
	: LogicalNode({std::move(lhs), std::move(rhs), opr}) {
	if (!OperatorAcceptor::acceptBinaryLogical(opr)) {
		throw ExpressionProcessorException("Expected binary logical operator.");
	}
}