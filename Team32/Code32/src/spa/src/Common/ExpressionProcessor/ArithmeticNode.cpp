#include "Common/ExpressionProcessor/ArithmeticNode.h"

#include <utility>

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

Common::ExpressionProcessor::ArithmeticNode::ArithmeticNode(MathematicalOperator opr, shared_ptr<AtomicNode> lhs,
                                                            shared_ptr<AtomicNode> rhs)
	: AtomicNode({std::move(lhs), std::move(rhs), opr}) {
	if (!OperatorAcceptor::acceptArithmetic(opr)) {
		throw ExpressionProcessorException("Expected arithmetic operator.");
	}
}