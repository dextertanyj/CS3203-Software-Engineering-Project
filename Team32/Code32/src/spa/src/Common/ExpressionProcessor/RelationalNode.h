#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_RELATIONALNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_RELATIONALNODE_H

#include <memory>

#include "Common/ExpressionProcessor/AtomicNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/LogicalNode.h"
#include "Common/TypeDefs.h"

class Common::ExpressionProcessor::RelationalNode : public LogicalNode {
public:
	RelationalNode(MathematicalOperator opr, std::shared_ptr<AtomicNode> lhs, std::shared_ptr<AtomicNode> rhs);
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_RELATIONALNODE_H
