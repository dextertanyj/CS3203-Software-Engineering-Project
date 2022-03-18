#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LOGICALNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LOGICALNODE_H

#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"

class Common::ExpressionProcessor::LogicalNode : public ExpressionNode {
public:
	explicit LogicalNode(vector<variant<shared_ptr<ExpressionNode>, MathematicalOperator, VarRef, ConstVal>> tokens)
		: ExpressionNode(tokens){};
	~LogicalNode() override = default;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LOGICALNODE_H
