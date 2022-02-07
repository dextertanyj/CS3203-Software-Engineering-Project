#ifndef SPA_VARIABLENODE_H
#define SPA_VARIABLENODE_H

#include "Common/ArithmeticProcessor/ArithmeticExpression.h"
#include "Common/ArithmeticProcessor/ExpressionNode.h"

class Common::ArithmeticProcessor::VariableNode : public ExpressionNode {
public:
	explicit VariableNode(VarRef name);
	bool equals(shared_ptr<ExpressionNode> object) override;
	bool contains(shared_ptr<ExpressionNode> object) override;
private:
	VarRef name;
};

#endif  // SPA_VARIABLENODE_H
