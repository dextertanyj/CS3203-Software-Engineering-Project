#include "Common/ExpressionProcessor/UnaryLogicalNode.h"

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;

Common::ExpressionProcessor::UnaryLogicalNode::UnaryLogicalNode(MathematicalOperator opr, shared_ptr<LogicalNode> expression)
	: LogicalNode({expression, opr}) {
	if (!OperatorAcceptor::acceptUnaryLogical(opr)) {
		throw ExpressionProcessorException("Expected unary logical operator");
	}
}