#ifndef SPA_ARITHMETICNODE_H
#define SPA_ARITHMETICNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "SP/Node/ArithmeticExpressionNode.h"

using namespace std;

class ArithmeticNode : public ArithmeticExpressionNode {
public:
	ArithmeticNode(ArithmeticOperator op, unique_ptr<ArithmeticExpressionNode> lhs, unique_ptr<ArithmeticExpressionNode> rhs);

private:
	ArithmeticOperator op;
	unique_ptr<ArithmeticExpressionNode> lhs;
	unique_ptr<ArithmeticExpressionNode> rhs;
};

#endif //SPA_ARITHMETICNODE_H
