#include "Common/ExpressionProcessor/RelationalNode.h"

#include <utility>

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

Common::ExpressionProcessor::RelationalNode::RelationalNode(MathematicalOperator opr, shared_ptr<AtomicNode> lhs,
                                                            shared_ptr<AtomicNode> rhs)
	: LogicalNode({std::move(lhs), std::move(rhs), opr}) {
	if (!OperatorAcceptor::acceptRelationalStrict(opr)) {
		throw ExpressionProcessorException("Expected relational operator.");
	}
}