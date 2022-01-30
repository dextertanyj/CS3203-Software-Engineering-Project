#ifndef SPA_NOTNODE_H
#define SPA_NOTNODE_H

#include <memory>

#include "SP/ConditionalExpressionNode.h"

using namespace std;

class NotNode : public ConditionalExpressionNode {
public:
	explicit NotNode(unique_ptr<ConditionalExpressionNode> expression);

private:
	unique_ptr<ConditionalExpressionNode> expression;
};


#endif //SPA_NOTNODE_H
