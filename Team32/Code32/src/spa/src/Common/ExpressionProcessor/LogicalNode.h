#ifndef SPA_LOGICALNODE_H
#define SPA_LOGICALNODE_H

#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/ExpressionNode.h"

class Common::ExpressionProcessor::LogicalNode : public ExpressionNode {
public:
	virtual ~LogicalNode() = default;
};

#endif  // SPA_LOGICALNODE_H
