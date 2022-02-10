#ifndef SPA_CONSTANTNODE_H
#define SPA_CONSTANTNODE_H

#include <memory>
#include <string>

#include "Common/ExpressionProcessor/AtomicNode.h"
#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"

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
