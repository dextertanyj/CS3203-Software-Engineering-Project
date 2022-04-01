#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_BINARYLOGICALNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_BINARYLOGICALNODE_H

#include <memory>

#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/LogicalNode.h"
#include "Common/TypeDefs.h"

class Common::ExpressionProcessor::BinaryLogicalNode : public LogicalNode {
public:
	BinaryLogicalNode(MathematicalOperator opr, shared_ptr<LogicalNode> lhs, shared_ptr<LogicalNode> rhs);
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_BINARYLOGICALNODE_H
