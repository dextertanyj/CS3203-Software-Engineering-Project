#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_ATOMICNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_ATOMICNODE_H

#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"

class Common::ExpressionProcessor::AtomicNode : public ExpressionNode {
public:
	~AtomicNode() override = default;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_ATOMICNODE_H
