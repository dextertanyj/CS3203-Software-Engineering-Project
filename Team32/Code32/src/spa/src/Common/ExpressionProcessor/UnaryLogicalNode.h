#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_UNARYLOGICALNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_UNARYLOGICALNODE_H

#include <memory>

#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/LogicalNode.h"
#include "Common/TypeDefs.h"

class Common::ExpressionProcessor::UnaryLogicalNode : public LogicalNode {
public:
	UnaryLogicalNode(MathematicalOperator opr, shared_ptr<LogicalNode> expression);
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_UNARYLOGICALNODE_H
