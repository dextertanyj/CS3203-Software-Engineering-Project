#ifndef SPA_BINARYLOGICALNODE_H
#define SPA_BINARYLOGICALNODE_H

#include "Common/ExpressionProcessor/LogicalNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"

class Common::ExpressionProcessor::BinaryLogicalNode : public LogicalNode {
public:
	BinaryLogicalNode(MathematicalOperator op, shared_ptr<LogicalNode> lhs, shared_ptr<LogicalNode> rhs);
	bool equals(shared_ptr<ExpressionNode> object) override;
	bool contains(shared_ptr<ExpressionNode> object) override;

private:
	MathematicalOperator op;
	shared_ptr<LogicalNode> lhs;
	shared_ptr<LogicalNode> rhs;
};

#endif  // SPA_BINARYLOGICALNODE_H
