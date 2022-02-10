#ifndef SPA_ATOMICNODE_H
#define SPA_ATOMICNODE_H

#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"

class Common::ExpressionProcessor::AtomicNode : public ExpressionNode {
public:
	virtual ~AtomicNode() = default;
};

#endif  // SPA_ATOMICNODE_H
