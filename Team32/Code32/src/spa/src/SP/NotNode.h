#ifndef SPA_NOTNODE_H
#define SPA_NOTNODE_H

#include "SP/ConditionalExpressionNode.h"

class NotNode : public ConditionalExpressionNode {
public:
	NotNode(ConditionalExpressionNode);

private:
	ConditionalExpressionNode expression;
};


#endif //SPA_NOTNODE_H
