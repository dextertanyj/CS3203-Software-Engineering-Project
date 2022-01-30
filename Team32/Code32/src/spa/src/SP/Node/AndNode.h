#ifndef SPA_ANDNODE_H
#define SPA_ANDNODE_H

#include <memory>

#include "ConditionalExpressionNode.h"

using namespace std;

class AndNode : public ConditionalExpressionNode {
public:
	AndNode(unique_ptr<ConditionalExpressionNode> lhs, unique_ptr<ConditionalExpressionNode> rhs);

private:
	unique_ptr<ConditionalExpressionNode> lhs;
	unique_ptr<ConditionalExpressionNode> rhs;
};


#endif //SPA_ANDNODE_H
