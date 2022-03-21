#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONNODE_H

#include <memory>
#include <variant>
#include <vector>

#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/TypeDefs.h"

class Common::ExpressionProcessor::ExpressionNode {
public:
	string traversal();
	virtual ~ExpressionNode() = default;

protected:
	ExpressionNode(vector<variant<shared_ptr<ExpressionNode>, MathematicalOperator, VarRef, ConstVal>> tokens);

private:
	vector<variant<shared_ptr<ExpressionNode>, MathematicalOperator, VarRef, ConstVal>> tokens;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONNODE_H
