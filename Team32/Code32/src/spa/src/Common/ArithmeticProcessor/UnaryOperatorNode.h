#ifndef SPA_UNARYOPERATORNODE_H
#define SPA_UNARYOPERATORNODE_H

#include <memory>

#include "Common/TypeDefs.h"
#include "Common/ArithmeticProcessor/ArithmeticProcessor.h"
#include "Common/ArithmeticProcessor/ExpressionNode.h"

using namespace std;

class Common::ArithmeticProcessor::UnaryOperatorNode : public ExpressionNode {
public:
	UnaryOperatorNode(MathematicalOperator op, shared_ptr<ExpressionNode> expression);
	bool equals(shared_ptr<ExpressionNode> object) override;
	bool contains(shared_ptr<ExpressionNode> object) override;
private:
	MathematicalOperator op;
	shared_ptr<ExpressionNode> expression;
};

#endif  // SPA_UNARYOPERATORNODE_H
