#ifndef SPA_ARITHMETICEXPRESSIONNODE_H
#define SPA_ARITHMETICEXPRESSIONNODE_H

#include <memory>

#include "SP/Node/RelationalFactorNode.h"

using namespace std;

class ArithmeticExpressionNode : public RelationalFactorNode {
public:
	static unique_ptr<ArithmeticExpressionNode> parseArithmeticExpression();
};

#endif  // SPA_ARITHMETICEXPRESSIONNODE_H
