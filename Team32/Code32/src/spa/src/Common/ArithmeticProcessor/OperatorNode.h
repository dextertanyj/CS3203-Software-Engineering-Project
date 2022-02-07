#ifndef SPA_OPERATORNODE_H
#define SPA_OPERATORNODE_H

#include <memory>

#include "Common/ArithmeticProcessor/ArithmeticProcessor.h"
#include "Common/ArithmeticProcessor/ExpressionNode.h"

using namespace std;

class Common::ArithmeticProcessor::OperatorNode : public ExpressionNode {
public:
	OperatorNode(ArithmeticOperator op, shared_ptr<ExpressionNode> lhs, shared_ptr<ExpressionNode> rhs);
	bool equals(shared_ptr<ExpressionNode> object) override;
	bool contains(shared_ptr<ExpressionNode> object) override;
private:
	ArithmeticOperator op;
	shared_ptr<ExpressionNode> lhs;
	shared_ptr<ExpressionNode> rhs;

};

#endif  // SPA_OPERATORNODE_H
