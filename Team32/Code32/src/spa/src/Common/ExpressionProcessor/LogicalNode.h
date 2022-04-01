#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LOGICALNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LOGICALNODE_H

#include <utility>
#include <vector>

#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"

class Common::ExpressionProcessor::LogicalNode : public ExpressionNode {
public:
	explicit LogicalNode(vector<TokenType> tokens) : ExpressionNode(move(tokens)){};
	~LogicalNode() override = default;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_LOGICALNODE_H
