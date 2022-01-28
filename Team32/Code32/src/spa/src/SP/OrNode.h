#ifndef SPA_ORNODE_H
#define SPA_ORNODE_H

#include "SP/ConditionalExpressionNode.h"

class OrNode : public ConditionalExpressionNode {
public:
	OrNode(ConditionalExpressionNode, ConditionalExpressionNode);

private:
	ConditionalExpressionNode lhs;
	ConditionalExpressionNode rhs;
};


#endif //SPA_ORNODE_H
