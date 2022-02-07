#ifndef SPA_ORNODE_H
#define SPA_ORNODE_H

#include <memory>

#include "SP/Node/ConditionalExpressionNode.h"
#include "SP/Node/Node.h"
#include "SP/SP.h"

using namespace std;

class SP::Node::OrNode : public ConditionalExpressionNode {
public:
	OrNode(unique_ptr<ConditionalExpressionNode> lhs, unique_ptr<ConditionalExpressionNode> rhs);
	UsageInfo extract() override;
	bool equals(shared_ptr<ConditionalExpressionNode> object) override;

private:
	unique_ptr<ConditionalExpressionNode> lhs;
	unique_ptr<ConditionalExpressionNode> rhs;
};

#endif  // SPA_ORNODE_H
