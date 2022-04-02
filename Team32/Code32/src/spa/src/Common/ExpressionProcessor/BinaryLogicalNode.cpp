#include "Common/ExpressionProcessor/BinaryLogicalNode.h"

#include <utility>

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

Common::ExpressionProcessor::BinaryLogicalNode::BinaryLogicalNode(MathematicalOperator opr, std::shared_ptr<LogicalNode> lhs,
                                                                  std::shared_ptr<LogicalNode> rhs)
	: LogicalNode({std::move(lhs), std::move(rhs), opr}) {
	if (!OperatorAcceptor::acceptBinaryLogical(opr)) {
		throw ExpressionProcessorException("Expected binary logical operator.");
	}
}