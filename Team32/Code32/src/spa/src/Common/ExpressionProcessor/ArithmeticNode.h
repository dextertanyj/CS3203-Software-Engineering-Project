#ifndef SPA_ARITHMETICNODE_H
#define SPA_ARITHMETICNODE_H

#include <memory>

#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/AtomicNode.h"

using namespace std;

class Common::ExpressionProcessor::ArithmeticNode : public AtomicNode {
public:
	ArithmeticNode(MathematicalOperator op, shared_ptr<AtomicNode> lhs, shared_ptr<AtomicNode> rhs);
	bool equals(shared_ptr<ExpressionNode> object) override;
	bool contains(shared_ptr<ExpressionNode> object) override;

private:
	MathematicalOperator op;
	shared_ptr<AtomicNode> lhs;
	shared_ptr<AtomicNode> rhs;
};

#endif  // SPA_ARITHMETICNODE_H
