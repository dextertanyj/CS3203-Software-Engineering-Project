#ifndef SPA_VARIABLENODE_H
#define SPA_VARIABLENODE_H

#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/AtomicNode.h"

class Common::ExpressionProcessor::VariableNode : public AtomicNode {
public:
	explicit VariableNode(VarRef name);
	bool equals(shared_ptr<ExpressionNode> object) override;
	bool contains(shared_ptr<ExpressionNode> object) override;
private:
	VarRef name;
};

#endif  // SPA_VARIABLENODE_H
