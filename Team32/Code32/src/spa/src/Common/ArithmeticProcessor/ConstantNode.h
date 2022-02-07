#ifndef SPA_CONSTANTNODE_H
#define SPA_CONSTANTNODE_H

#include "Common/ArithmeticProcessor/ArithmeticProcessor.h"
#include "Common/ArithmeticProcessor/ExpressionNode.h"

using namespace std;

class Common::ArithmeticProcessor::ConstantNode : public ExpressionNode {
public:
	explicit ConstantNode(const string& value);
	bool equals(shared_ptr<ExpressionNode> object) override;
	bool contains(shared_ptr<ExpressionNode> object) override;
private:
	int value;
};

#endif  // SPA_CONSTANTNODE_H
