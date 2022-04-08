#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSION_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSION_H

#include <memory>
#include <string>

#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/TypeDefs.h"

class Common::ExpressionProcessor::Expression {
public:
	Expression(std::shared_ptr<ExpressionNode> root, VarRefSet variables, ConstValSet constants);
	[[nodiscard]] ConstValSet getConstants() const;
	[[nodiscard]] VarRefSet getVariables() const;
	[[nodiscard]] bool operator==(const Expression& other) const;
	[[nodiscard]] bool contains(const Expression& other) const;

	friend std::hash<Expression>;

private:
	std::shared_ptr<ExpressionNode> root;
	std::string traversal;
	VarRefSet variables;
	ConstValSet constants;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSION_H
