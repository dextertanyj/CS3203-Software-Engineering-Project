#include "Common/ExpressionProcessor/BinaryLogicalNode.h"

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;

Common::ExpressionProcessor::BinaryLogicalNode::BinaryLogicalNode(MathematicalOperator opr, shared_ptr<LogicalNode> lhs,
                                                                  shared_ptr<LogicalNode> rhs)
	: LogicalNode({lhs, rhs, opr}) {
	if (!OperatorAcceptor::acceptBinaryLogical(opr)) {
		throw ExpressionProcessorException("Expected binary logical operator.");
	}
}