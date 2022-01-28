#ifndef SPA_ARITHMETICNODE_H
#define SPA_ARITHMETICNODE_H

#include "Common/TypeDefs.h"
#include "SP/ArithmeticExpressionNode.h"

class ArithmeticNode : public ArithmeticExpressionNode {
public:
	ArithmeticNode(ArithmeticOperator, ArithmeticExpressionNode, ArithmeticExpressionNode);

private:
	ArithmeticOperator op;
	ArithmeticExpressionNode lhs;
	ArithmeticExpressionNode rhs;
};

#endif //SPA_ARITHMETICNODE_H
