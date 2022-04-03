#ifndef SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSION_H
#define SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSION_H

#include <memory>
#include <string>
#include <unordered_set>
#include <variant>

#include "Common/ExpressionProcessor/ExpressionNode.h"
#include "Common/ExpressionProcessor/ExpressionProcessor.h"
#include "Common/TypeDefs.h"

class Common::ExpressionProcessor::Expression {
public:
	Expression(std::shared_ptr<ExpressionNode> root, std::unordered_set<VarRef> variables, std::unordered_set<ConstVal> constants);
	[[nodiscard]] std::unordered_set<ConstVal> getConstants() const;
	[[nodiscard]] std::unordered_set<VarRef> getVariables() const;
	[[nodiscard]] bool operator==(const Expression& other) const;
	[[nodiscard]] bool contains(const Expression& other) const;

private:
	std::shared_ptr<ExpressionNode> root;
	std::string traversal;
	std::unordered_set<VarRef> variables;
	std::unordered_set<ConstVal> constants;
};

#endif  // SPA_SRC_COMMON_EXPRESSIONPROCESSOR_EXPRESSION_H
