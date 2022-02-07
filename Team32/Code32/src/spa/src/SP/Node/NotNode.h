#ifndef SPA_NOTNODE_H
#define SPA_NOTNODE_H

#include <memory>

#include "SP/Node/ConditionalExpressionNode.h"

using namespace std;

class NotNode : public ConditionalExpressionNode {
public:
	explicit NotNode(unique_ptr<ConditionalExpressionNode> expression);
	UsageInfo extract() override;
    bool equals(shared_ptr<ConditionalExpressionNode> object) override;

private:
	unique_ptr<ConditionalExpressionNode> expression;
};

#endif  // SPA_NOTNODE_H
