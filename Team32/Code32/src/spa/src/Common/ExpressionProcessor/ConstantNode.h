#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_CONSTANTNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_CONSTANTNODE_H

#include <memory>
#include <string>

#include "Common/ExpressionProcessor/AtomicNode.h"
#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/TypeDefs.h"

using namespace std;

class Common::ExpressionProcessor::ConstantNode : public AtomicNode {
public:
	explicit ConstantNode(const string& value);
	bool equals(shared_ptr<ExpressionNode> object) override;
	bool contains(shared_ptr<ExpressionNode> object) override;

private:
	ConstVal value;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_CONSTANTNODE_H
