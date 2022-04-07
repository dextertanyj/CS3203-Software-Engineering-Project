#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONNODE_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONNODE_H

#include <variant>
#include <vector>

#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/TypeDefs.h"

class Common::ExpressionProcessor::ExpressionNode {
public:
	std::string traversal();
	virtual ~ExpressionNode() = default;

protected:
	using TokenType = std::variant<std::shared_ptr<ExpressionNode>, MathematicalOperator, VarRef, ConstVal>;
	explicit ExpressionNode(std::vector<TokenType> tokens);

private:
	std::vector<TokenType> tokens;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSIONNODE_H
