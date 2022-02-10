#ifndef SPA_RELATIONALOPERATORNODE_H
#define SPA_RELATIONALOPERATORNODE_H

#include <memory>

#include "Common/ExpressionProcessor/AtomicNode.h"
#include "Common/ExpressionProcessor/LogicalNode.h"
#include "Common/TypeDefs.h"

class Common::ExpressionProcessor::RelationalNode : public LogicalNode {
public:
	RelationalNode(MathematicalOperator op, shared_ptr<AtomicNode> lhs, shared_ptr<AtomicNode> rhs);
	bool equals(shared_ptr<ExpressionNode> object) override;
	bool contains(shared_ptr<ExpressionNode> object) override;

private:
	MathematicalOperator op;
	shared_ptr<AtomicNode> lhs;
	shared_ptr<AtomicNode> rhs;
};

#endif  // SPA_RELATIONALOPERATORNODE_H
