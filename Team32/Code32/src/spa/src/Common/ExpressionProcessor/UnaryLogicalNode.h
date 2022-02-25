#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_UNARYLOGICALNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_UNARYLOGICALNODE_H

#include <memory>

#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/LogicalNode.h"
#include "Common/TypeDefs.h"

using namespace std;

class Common::ExpressionProcessor::UnaryLogicalNode : public LogicalNode {
public:
	UnaryLogicalNode(MathematicalOperator op, shared_ptr<LogicalNode> expression);
	bool equals(shared_ptr<ExpressionNode> object) override;
	bool contains(shared_ptr<ExpressionNode> object) override;

private:
	MathematicalOperator op;
	shared_ptr<LogicalNode> expression;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_UNARYLOGICALNODE_H
