#ifndef SPA_ANDNODE_H
#define SPA_ANDNODE_H

#include "SP/ConditionalExpressionNode.h"

class AndNode : public ConditionalExpressionNode {
public:
	AndNode(ConditionalExpressionNode lhs, ConditionalExpressionNode rhs);

private:
	ConditionalExpressionNode lhs;
	ConditionalExpressionNode rhs;
};


#endif //SPA_ANDNODE_H
