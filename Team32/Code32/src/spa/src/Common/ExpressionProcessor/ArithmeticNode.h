#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_ARITHMETICNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_ARITHMETICNODE_H

#include <memory>

#include "Common/ExpressionProcessor/AtomicNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/TypeDefs.h"

class Common::ExpressionProcessor::ArithmeticNode : public AtomicNode {
public:
	ArithmeticNode(MathematicalOperator opr, std::shared_ptr<AtomicNode> lhs, std::shared_ptr<AtomicNode> rhs);
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_ARITHMETICNODE_H
