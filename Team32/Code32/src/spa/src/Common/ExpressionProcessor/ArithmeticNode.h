#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_ARITHMETICNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_ARITHMETICNODE_H

#include <memory>

#include "Common/ExpressionProcessor/AtomicNode.h"
#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/TypeDefs.h"

using namespace std;

class Common::ExpressionProcessor::ArithmeticNode : public AtomicNode {
public:
	ArithmeticNode(MathematicalOperator opr, shared_ptr<AtomicNode> lhs, shared_ptr<AtomicNode> rhs);
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_ARITHMETICNODE_H
