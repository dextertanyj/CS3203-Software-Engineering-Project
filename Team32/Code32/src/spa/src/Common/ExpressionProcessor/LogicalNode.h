#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LOGICALNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LOGICALNODE_H

#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"

class Common::ExpressionProcessor::LogicalNode : public ExpressionNode {
public:
	~LogicalNode() override = default;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LOGICALNODE_H
