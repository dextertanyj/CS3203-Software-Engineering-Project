#ifndef SPA_CONSTANTNODE_H
#define SPA_CONSTANTNODE_H

#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/ExpressionProcessor/AtomicNode.h"

using namespace std;

class Common::ExpressionProcessor::ConstantNode : public AtomicNode {
public:
	explicit ConstantNode(const string& value);
	bool equals(shared_ptr<ExpressionNode> object) override;
	bool contains(shared_ptr<ExpressionNode> object) override;
private:
	int value;
};

#endif  // SPA_CONSTANTNODE_H
