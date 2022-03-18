#include "Common/ExpressionProcessor/ArithmeticNode.h"

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;

Common::ExpressionProcessor::ArithmeticNode::ArithmeticNode(MathematicalOperator opr, shared_ptr<AtomicNode> lhs,
                                                            shared_ptr<AtomicNode> rhs)
	: AtomicNode({lhs, rhs, opr}) {
	if (!OperatorAcceptor::acceptArithmetic(opr)) {
		throw ExpressionProcessorException("Expected arithmetic operator.");
	}
}