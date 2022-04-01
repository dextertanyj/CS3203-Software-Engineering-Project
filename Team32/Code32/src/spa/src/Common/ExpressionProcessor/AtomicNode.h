#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_ATOMICNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_ATOMICNODE_H

#include <utility>

#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"

class Common::ExpressionProcessor::AtomicNode : public ExpressionNode {
public:
	explicit AtomicNode(vector<variant<shared_ptr<ExpressionNode>, MathematicalOperator, VarRef, ConstVal>> tokens)
		: ExpressionNode(std::move(tokens)){};
	~AtomicNode() override = default;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_ATOMICNODE_H
