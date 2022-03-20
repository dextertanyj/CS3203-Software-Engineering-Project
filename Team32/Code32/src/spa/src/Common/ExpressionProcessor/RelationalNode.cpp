#include "Common/ExpressionProcessor/RelationalNode.h"

#include "Common/ExpressionProcessor/OperatorAcceptor.h"

using namespace std;

Common::ExpressionProcessor::RelationalNode::RelationalNode(MathematicalOperator opr, shared_ptr<AtomicNode> lhs,
                                                            shared_ptr<AtomicNode> rhs)
	: LogicalNode({lhs, rhs, opr}) {
	if (!OperatorAcceptor::acceptRelationalStrict(opr)) {
		throw ExpressionProcessorException("Expected relational operator.");
	}
}