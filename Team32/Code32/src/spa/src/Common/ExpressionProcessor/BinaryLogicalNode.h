#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_BINARYLOGICALNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_BINARYLOGICALNODE_H

#include <memory>

#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/LogicalNode.h"
#include "Common/TypeDefs.h"

using namespace std;

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

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_BINARYLOGICALNODE_H
